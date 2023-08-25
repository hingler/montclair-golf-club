#ifndef COURSE_SMOOTHER_H_
#define COURSE_SMOOTHER_H_

#include <algorithm>

// debug :3
#include <iostream>

#include <memory>

#include "course/sampler/GaussianMetaballSampler.hpp"
#include "course/path/CompoundCurve.hpp"
#include "type/sampler_type.hpp"

#include "util/SamplerTools.hpp"

#include <glm/glm.hpp>

namespace terrain {
  /**
   * @brief Smooths out terrain in proximity of our course
   * 
   * @tparam LoFreqType - sampler containing low frequency information
   * @tparam HiFreqType - sampler containing lo+hi frequency information
   */
  template <typename LoFreqType, typename HiFreqType>
  class CourseSmoother {
    // verify typing
    static_assert(type::sampler_type<LoFreqType>::value);
    static_assert(type::sampler_type<HiFreqType>::value);

   public:
    // implement sample spec
    // lazy init scale on first sample
    // based on distance from course features (estimate w gauss sample), fade into hi freq + fade out scale
    // - sdf wrap won't work (can't get distance info atm), just need to handle here
    // (tba) if near green, flatten course (prob need a separate sampler for green??)
    CourseSmoother(
      std::shared_ptr<LoFreqType> lo_freq, 
      std::shared_ptr<HiFreqType> hi_freq,
      std::shared_ptr<course::sampler::GaussianMetaballSampler> course_sampler,
      course::path::CompoundCurve& compound_curve
    ) : lo_freq_(lo_freq), hi_freq_(hi_freq), course_sampler_(course_sampler), curve_(compound_curve) {}

    double Sample(double x, double y) {
      if (!init_flag) {
        GenerateHeightScale();
      }

      // when sampling: take ln bc otherwise it'll come out funny
      double course_sample_log = log(course_sampler_->Sample(x, y));
      double hi_freq_sample = hi_freq_->Sample(x, y);
      double lo_freq_sample = lo_freq_->Sample(x, y);

      // 1.0: use lo freq
      // 0.0: use hi freq
      double sample_t = glm::smoothstep(FADE_END_LOG, FADE_START_LOG, course_sample_log);
      // scale down
      double lo_freq_scale = sample_t * lo_freq_height_scale;
      double hi_freq_scale = (1.0 - sample_t);

      // squash about height_origin
      lo_freq_sample = ((lo_freq_sample - height_origin) * lo_freq_scale) + height_origin;
      hi_freq_sample = ((hi_freq_sample - height_origin) * hi_freq_scale) + height_origin;
      // sampling
      // - generate height context if not already done
      // - crunch course mb
      // - sample low
      // - if below threshold: smoothstep into hi freq map
      return lo_freq_sample + hi_freq_sample;
    }
   private:
    // alt: use one terrain map, and compute this ourselves??
    // shrug - we can just pass the same pointer for both if we need it :3
    // (want to figure out how this looks first)
    std::shared_ptr<LoFreqType> lo_freq_;
    std::shared_ptr<HiFreqType> hi_freq_;
    std::shared_ptr<course::sampler::GaussianMetaballSampler> course_sampler_;
    course::path::CompoundCurve curve_;

    // computes scale for terrain height
    void GenerateHeightScale() {
      glm::dvec2 local_gradient;
      double max_gradient;

      double sampled_value;
      double rolling_sampled_value;
      size_t sample_count;

      double fade_start_exp = exp(FADE_START_LOG);

      for (double i = 0.0; i < 1.0; i += TIME_STEP) {
        glm::dvec2 curve_pos = static_cast<glm::dvec2>(curve_.Sample(i));
        glm::dvec2 tan = static_cast<glm::dvec2>(curve_.Tangent(i));
        // get normal vector
        glm::dvec2 normal = glm::dvec2(tan.y, -tan.x);
        glm::dvec2 inv_normal = glm::dvec2(-normal.x, -normal.y);

        glm::dvec2 rolling_pos = curve_pos;

        // roll gradient in one direction
        do {
          sampled_value = lo_freq_->Sample(rolling_pos.x, rolling_pos.y);
          rolling_sampled_value += sampled_value;
          sample_count++;
          rolling_pos += normal * GRADIENT_STEP;
          local_gradient = util::Gradient(*lo_freq_.get(), rolling_pos.x, rolling_pos.y);
          max_gradient = std::max(max_gradient, glm::length(local_gradient));
        } while (course_sampler_->Sample(rolling_pos.x, rolling_pos.y) > fade_start_exp);

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
        } while (course_sampler_->Sample(rolling_pos.x, rolling_pos.y) > fade_start_exp);

        // missing a few spots - but should be fine for now

      }

      // set origin to mean height (we'll scale up/down around this point)
      double mean_height = (rolling_sampled_value / sample_count);
      height_origin = mean_height;
      
      lo_freq_height_scale = std::clamp(GRADIENT_MAX / max_gradient, 0.0, 1.0);

      init_flag = true;
    std::cout << "height origin: " << height_origin << " // lo freq scale: " << lo_freq_height_scale << std::endl;
    }


    // scaling params (lazy init later)

    // height origin which terrain is scaled from
    double height_origin = 0.0;

    // scale applied to height in proximity of course terrain
    double lo_freq_height_scale = 1.0;

    // flag indicating whether we have initialized scaling params
    bool init_flag = false;

    // tba params
    // - proximity threshold (mag of gaussian)
    // - proximity limit (return to normal scaled gaussian)

    // gonna hard code these in cpp for now

    // this works for now - but i really really need to speed this up (eventually)

    constexpr static double FADE_START_LOG = -4.0;
    constexpr static double FADE_END_LOG = -25.0;

    constexpr static double GRADIENT_STEP = 2.0;
    constexpr static double TIME_STEP = 0.005;

    // ~6 percent slope will be our threshold for now
    constexpr static double GRADIENT_MAX = 0.000;
  };
}

#endif // COURSE_SMOOTHER_H_