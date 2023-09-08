#include "course/sampler/terrain/SimpleCourseTerrainSampler.hpp"

#include <glm/glm.hpp>

namespace course {
  namespace sampler {
    namespace terrain {
      // need to specify an origin
      SimpleCourseTerrainSampler::SimpleCourseTerrainSampler() : SimpleCourseTerrainSampler(glm::dvec2(0.0, 0.0)) {

      }

      SimpleCourseTerrainSampler::SimpleCourseTerrainSampler(const glm::dvec2& origin)
       : high_frequency_course_sampler(glm::vec3(36.0f), 2),
         mid_frequency_course_sampler(glm::vec3(128.0f), 2),
         low_frequency_course_sampler(glm::vec3(512.0f), 2),
         infra_frequency_course_sampler(glm::vec3(3072.0f), 2),
         origin_(origin) {
      }

      float SimpleCourseTerrainSampler::Sample(float x, float y) const {
        float acc = 0.0f;
        // we evaluate distance from origin (origin is where we want to concentrate detail)
        x -= 512.0;
        y -= 512.0;

        double dist_from_origin = glm::length(glm::dvec2(x - origin_.x, y - origin_.y));

        double high_amplitude_mag = 1.0 - glm::smoothstep(768.0, 1536.0, dist_from_origin);
        double mid_amplitude_mag = 1.0 - glm::smoothstep(1536.0, 3072.0, dist_from_origin);
        acc += high_frequency_course_sampler.Sample(x, y) * 2.5f * high_amplitude_mag;
        acc += mid_frequency_course_sampler.Sample(x, y) * 1.4f * mid_amplitude_mag;
        // want the peaks here to be sharper, less frequent
        float low_freq = low_frequency_course_sampler.Sample(x, y) + 3.6f;
        acc += (glm::smoothstep(0.2f, 0.8f, low_freq) * 0.8 + low_freq * 0.2) * 3.5f;
        acc += infra_frequency_course_sampler.Sample(x, y) * 5.5f;
        acc *= 12.0;
        return acc;
      }
    }
  }
}