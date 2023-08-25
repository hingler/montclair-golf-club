#include "course/sampler/gaussian/GaussianSandSampler.hpp"

#include <iostream>

namespace course {
  namespace sampler {
    namespace gaussian {

      // max number of gradient steps to take
      static const size_t STEP_MAX = 512;

      GaussianSandSampler::GaussianSandSampler(
        GaussianMetaballSampler& fairway_sampler,
        GaussianMetaballSampler& hazard_sampler,
        const path::CoursePath& course_path,
        const path::CompoundCurve& bezier_curve
      ) : fairway_sampler(fairway_sampler),
          hazard_sampler(hazard_sampler),
          path(course_path),
          curve(bezier_curve) {}

      void GaussianSandSampler::Generate(const GaussianPathConfig& config) {
        engine.seed(arc4random());
        auto& sand_config = config.sand_config;

        // based on generation factor:
        std::uniform_real_distribution<double> f_dist(0.0, 1.0);
        
        std::uniform_real_distribution<double> theta_dist(0.0, 2 * M_PI);

        // wider generation gamut
        int generation_count = static_cast<int>(sand_config.generation_factor * (0.7 * f_dist(engine) + 0.5));

        for (int i = 0; i < generation_count; i++) {
          double min_t = curve.GetTimeForEndOfSpecifiedSegment(0);
          double sample_t = f_dist(engine) * (1.0 - min_t) + min_t;
          double nearest_t = curve.GetTimeForEndOfSpecifiedSegment(0);
          for (int j = 0; j < path.course_path.size() - 1; j++) {
            if (abs(curve.GetTimeForEndOfSpecifiedSegment(j) - sample_t) < abs(sample_t - nearest_t)) {
              nearest_t = curve.GetTimeForEndOfSpecifiedSegment(j);
            }
          }

          sample_t = sample_t * (1.0 - sand_config.tightness) + nearest_t * (sand_config.tightness);

          glm::vec2 sample_point = curve.Sample(sample_t);

          // deviate slightly to kick gradient into gear
          sample_point.x += (f_dist(engine) - 0.5);
          sample_point.y += (f_dist(engine) - 0.5);

          std::cout << "sample t: " << sample_t << std::endl;

          glm::vec2 gradient = GetNetGradient(sample_point);
          size_t step_count = 0;
          while (
            (GetNetSample(sample_point) > sand_config.gradient_threshold) 
            && (gradient.length() > 0.125)
            && (step_count < STEP_MAX)) {
            // move away from gradient direction!
            std::cout << gradient.x << ", " << gradient.y << std::endl;
            sample_point -= glm::normalize(gradient) * static_cast<float>(sand_config.step_factor);
            gradient = GetNetGradient(sample_point);
            step_count++;
          }

          if (GetNetSample(sample_point) <= sand_config.gradient_threshold) {
            // place sand balls
            // TODO: introduce aspect ratio to favor placing along gradient??
            for (int coarse_sample = 0; coarse_sample < sand_config.coarse_density; coarse_sample++) {
              glm::vec2 coarse_sample_point = sample_point;
              double displacement_theta = theta_dist(engine);
              double displacement_radius = sqrt(f_dist(engine)) * sand_config.coarse_scatter_dist;
              coarse_sample_point += glm::vec2(cos(displacement_theta) * displacement_radius, sin(displacement_theta) * displacement_radius);
              
              for (int fine_sample = 0; fine_sample < sand_config.fine_density; fine_sample++) {
                glm::vec2 offset_sample_point = coarse_sample_point + glm::vec2(1.0 - 2.0 * f_dist(engine), 1.0 - 2.0 * f_dist(engine)) * static_cast<float>(sand_config.fine_scatter_radius);
                hazard_sampler.Add(offset_sample_point.x, offset_sample_point.y, sand_config.sigma, sand_config.sigma * sand_config.divot_intensity);
              }
            }
          }

          // generally works as-is - want to step along terrain lines to build more "interesting" sand patches
          // tba: water :)
        }
      }

      glm::vec2 GaussianSandSampler::GetNetGradient(const glm::vec2& sample_point) {
        return fairway_sampler.Gradient(sample_point.x, sample_point.y) + hazard_sampler.Gradient(sample_point.x, sample_point.y);
      }

      double GaussianSandSampler::GetNetSample(const glm::vec2& sample_point) {
        return fairway_sampler.Sample(sample_point.x, sample_point.y) + hazard_sampler.Sample(sample_point.x, sample_point.y);
      }
    }
  }
}