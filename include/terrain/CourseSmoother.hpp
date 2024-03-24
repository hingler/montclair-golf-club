#ifndef COURSE_SMOOTHER_H_
#define COURSE_SMOOTHER_H_

#define SAMPLE_EPS 0.000001

#include <algorithm>

// debug :3
#include <iostream>

#include <atomic>
#include <memory>
#include <mutex>

#include "course/sampler/GaussianMetaballSampler.hpp"
#include "course/path/CompoundCurve.hpp"
#include "type/sampler_type.hpp"

#include "util/SamplerTools.hpp"

#include <glm/glm.hpp>

namespace gdterrain {

  // can probably estimate course edges here

  /**
   * @brief Smooths out terrain in proximity of our course
   * 
   * @tparam LoFreqType - sampler containing low frequency information
   * @tparam HiFreqType - sampler containing lo+hi frequency information
   */
  template <typename LoFreqType, typename HiFreqType, typename DistantFreqType>
  class CourseSmoother {
    // verify typing
    static_assert(type::sampler_type<LoFreqType>::value);
    static_assert(type::sampler_type<HiFreqType>::value);
    static_assert(type::sampler_type<DistantFreqType>::value);

   public:
    // implement sample spec
    // lazy init scale on first sample
    // based on distance from course features (estimate w gauss sample), fade into hi freq + fade out scale
    // - sdf wrap won't work (can't get distance info atm), just need to handle here
    // (tba) if near green, flatten course (prob need a separate sampler for green??)
    CourseSmoother(
      std::shared_ptr<LoFreqType> lo_freq, 
      std::shared_ptr<HiFreqType> hi_freq,
      std::shared_ptr<DistantFreqType> distant_freq,
      std::shared_ptr<course::sampler::GaussianMetaballSampler> course_sampler,
      course::path::CompoundCurve& compound_curve
    ) : lo_freq_(lo_freq), hi_freq_(hi_freq), distant_freq_(distant_freq), course_sampler_(course_sampler), curve_(compound_curve), init_flag(false) 
    {
      GenerateHeightScale();
    }

    // try copying underlying samplers!
    CourseSmoother(CourseSmoother& other)
    : lo_freq_(std::make_shared<LoFreqType>(*other.lo_freq_)),
      hi_freq_(std::make_shared<HiFreqType>(*other.hi_freq_)),
      distant_freq_(std::make_shared<DistantFreqType>(*other.distant_freq_)),
      course_sampler_(other.course_sampler_),
      curve_(other.curve_),
      init_flag(false)
    {
      // not marked as const so we can do this if we need to
      if (!other.init_flag.load()) {
        // generate before copying
        other.GenerateHeightScale();
      }

      course_center = other.course_center;
      distant_radius = other.distant_radius;

      height_origin = other.height_origin;
      lo_freq_height_scale = other.lo_freq_height_scale;
      init_flag = true;
    }

    float Get(double x, double y) {
      return Sample(static_cast<double>(x), static_cast<double>(y));
    }

    glm::dvec2 GetCourseOrigin() {
      if (!init_flag) {
        GenerateHeightScale();
      }

      return course_min;
    }

    glm::dvec2 GetCourseSize() {
      if (!init_flag) {
        GenerateHeightScale();
      }

      // should be fine

      return (course_max - course_min);
    }

    glm::dvec3 GetCourseCenter() {
      if (!init_flag) {
        GenerateHeightScale();
      }

      // midpoint of course path - should be more acc?????
      // (we'll see :3)
      auto xz_center = curve_.Sample(0.5);
      // ensures it's on the ground :3
      return glm::dvec3(xz_center.x, Sample(xz_center.x, xz_center.y), xz_center.y);
    }

    double Sample(double x, double y) const {
      if (!init_flag.load()) {
        // only grab lock if condition initially fails
        std::lock_guard lock(height_mutex);
        if (!init_flag.load()) {
          // workaround for caching
          const_cast<CourseSmoother*>(this)->GenerateHeightScale();
        }
      }

      double center_dist = (glm::dvec2(x, y) - course_center).length();

      // when sampling: take ln bc otherwise it'll come out funny
      // (still costly, but definitely not as bad as it was)

      // this (iirc) is costly, bc we had to sample that metaball thing
      // yeah like 20% the runtime lole
      // - 10k -> 30k us : 60 -> 80k us - over 4096 samples: cost of about 12us on sampling
      // - that's *about* the delay we were seeing on the sampler?
      // - (i think we were getting like 100 rows per sample, like 200 - 400k samples? a good bit quicker ig)
      double course_sample_log = log(course_sampler_->Sample(x, y));
      double distant_freq_sample = 0.0;
      double hi_freq_sample = 0.0;
      double lo_freq_sample = 0.0;

      // 1.0: use lo freq
      // 0.0: use hi freq

      // trying using radius instead of course sample, to speed things up a bit
      double sample_t = glm::smoothstep(FADE_END_LOG, FADE_START_LOG, course_sample_log);

      double lo_freq_scale = sample_t * lo_freq_height_scale;
      double hi_freq_scale = (1.0 - sample_t);

      if (sample_t > SAMPLE_EPS) {
        lo_freq_sample = lo_freq_->Sample(x, y);
      } 

      // in the busiest cases: this gets resampled
      if (hi_freq_scale > SAMPLE_EPS) {
        if (hi_freq_ == lo_freq_ && sample_t > SAMPLE_EPS) {
          hi_freq_sample = lo_freq_sample;
        } else {
          hi_freq_sample = hi_freq_->Sample(x, y);
        }

      }

      double dist = glm::length(glm::dvec2(x, y) - course_center);
      double distant_weight = glm::smoothstep(5.0 * distant_radius, 15.0 * distant_radius, dist);

      // 3x speed up lmao
      if (distant_weight > SAMPLE_EPS) {
        if (distant_freq_ == lo_freq_ && sample_t > SAMPLE_EPS) {
          distant_freq_sample = lo_freq_sample;
        } else {
          distant_freq_sample = distant_freq_->Sample(x, y);
        }
      }

      // squash about height_origin
      lo_freq_sample = ((lo_freq_sample - height_origin) * lo_freq_scale) + height_origin;
      hi_freq_sample = ((hi_freq_sample - height_origin) * hi_freq_scale) + height_origin;
      // should be independent - ie don't trump these for now
      distant_freq_sample = (distant_freq_sample - height_origin) * distant_weight + height_origin;
      // sampling
      // - generate height context if not already done
      // - crunch course mb
      // - sample low
      // - if below threshold: smoothstep into hi freq map

      // there NEEDS to be a better way to implement this smoothing behavior - majority of the time is just course sampling

      // calc distant
      return lo_freq_sample + hi_freq_sample + distant_freq_sample;
    }
   private:
    // alt: use one terrain map, and compute this ourselves??
    // shrug - we can just pass the same pointer for both if we need it :3
    // (want to figure out how this looks first)
    std::shared_ptr<LoFreqType> lo_freq_;
    std::shared_ptr<HiFreqType> hi_freq_;
    std::shared_ptr<DistantFreqType> distant_freq_;
    std::shared_ptr<course::sampler::GaussianMetaballSampler> course_sampler_;
    course::path::CompoundCurve curve_;

    // need to inspect this shit
   public:
    glm::dvec2 course_center;
    double distant_radius;

    // height origin which terrain is scaled from
    double height_origin = 0.0;

    // scale applied to height in proximity of course terrain
    double lo_freq_height_scale = 1.0;

    // flag indicating whether we have initialized scaling params
    mutable std::atomic_bool init_flag;

    mutable std::mutex height_mutex;


    // computes scale for terrain height
    void GenerateHeightScale() {
      // lock while generating
      std::lock_guard<std::mutex> lock(height_mutex);
      glm::dvec2 local_gradient;
      double max_gradient;

      double sampled_value;
      double rolling_sampled_value = 0;
      size_t sample_count = 0;

      double fade_start_exp = exp(FADE_START_LOG);

      glm::dvec2 rolling_sum(0.0);
      size_t course_sample_count = 0;

      glm::dvec2 min_bound = curve_.Sample(0.0);
      glm::dvec2 max_bound = min_bound;


      for (double i = 0.0; i < 1.0; i += TIME_STEP) {
        course_sample_count++;
        glm::dvec2 curve_pos = static_cast<glm::dvec2>(curve_.Sample(i));
        std::cout << curve_pos.x << ", " << curve_pos.y << std::endl;
        rolling_sum += curve_pos;
        glm::dvec2 tan = static_cast<glm::dvec2>(curve_.Tangent(i));
        // get normal vector
        glm::dvec2 normal = glm::dvec2(tan.y, -tan.x);
        glm::dvec2 inv_normal = glm::dvec2(-normal.x, -normal.y);

        glm::dvec2 rolling_pos = curve_pos;

        // roll gradient in one direction
        // crunch course dims here
        do {
          sampled_value = lo_freq_->Sample(rolling_pos.x, rolling_pos.y);
          rolling_sampled_value += sampled_value;
          sample_count++;
          rolling_pos += normal * GRADIENT_STEP;
          local_gradient = util::Gradient(*lo_freq_.get(), rolling_pos.x, rolling_pos.y);
          max_gradient = std::max(max_gradient, glm::length(local_gradient));

          {
            // check bounds
            min_bound = glm::min(min_bound, rolling_pos);
            max_bound = glm::max(max_bound, rolling_pos);
          }

        } while (course_sampler_->Sample(rolling_pos.x, rolling_pos.y) > fade_start_exp);
        // on bordewr

        rolling_pos = curve_pos;

        // roll gradient in the other direction
        do {
          // redundant code :3
          sampled_value = lo_freq_->Sample(rolling_pos.x, rolling_pos.y);
          rolling_sampled_value += sampled_value;
          sample_count++;
          rolling_pos += inv_normal * GRADIENT_STEP;
          local_gradient = util::Gradient(*lo_freq_.get(), rolling_pos.x, rolling_pos.y);
          max_gradient = std::max(max_gradient, glm::length(local_gradient));

          {
            // check bounds
            min_bound = glm::min(min_bound, rolling_pos);
            max_bound = glm::max(max_bound, rolling_pos);
          }

        } while (course_sampler_->Sample(rolling_pos.x, rolling_pos.y) > fade_start_exp);
        min_bound = glm::min(min_bound, rolling_pos);
        max_bound = glm::max(max_bound, rolling_pos);
        // missing a few spots - but should be fine for now
      }

      // rough estimate of course center
      course_center = rolling_sum / static_cast<double>(course_sample_count);
      auto bb = curve_.GetBoundingBox();
      std::cout << "bb: " << bb.start.x << ", " << bb.start.y << " to " << bb.end.x << ", " << bb.end.y << std::endl;
      // i kinda did it here
      // conservative estimate
      distant_radius = glm::length(bb.end - bb.start) * 0.8;
      std::cout << "radius: " << distant_radius << std::endl;

      // set origin to mean height (we'll scale up/down around this point)
      double mean_height = (rolling_sampled_value / sample_count);
      height_origin = mean_height;
      if (std::abs(max_gradient) < 0.0001) {
        max_gradient = 0.0001;
      }
      lo_freq_height_scale = std::clamp(GRADIENT_MAX / max_gradient, 0.0, 1.0);

      // prep min and max
      course_min = min_bound;
      course_max = max_bound;

      init_flag = true;
      std::cout << "height origin: " << height_origin << " // lo freq scale: " << lo_freq_height_scale << std::endl;
    }


    // scaling params (lazy init later)
    // min/max dimension estimates for the course itself
    glm::dvec2 course_min;
    glm::dvec2 course_max;

    // tba params
    // - proximity threshold (mag of gaussian)
    // - proximity limit (return to normal scaled gaussian)

    // gonna hard code these in cpp for now

    // this works for now - but i really really need to speed this up (eventually)

    constexpr static double FADE_START_LOG = -3.0;
    constexpr static double FADE_END_LOG = -60.0;

    constexpr static double GRADIENT_STEP = 2.0;
    constexpr static double TIME_STEP = 0.005;

    // ~6 percent slope will be our threshold for now
    constexpr static double GRADIENT_MAX = 0.06;
  };
}

#endif // COURSE_SMOOTHER_H_