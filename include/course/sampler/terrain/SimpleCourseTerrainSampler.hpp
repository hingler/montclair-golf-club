#ifndef COURSE_TERRAIN_SAMPLER_H_
#define COURSE_TERRAIN_SAMPLER_H_

// todo: convert this to a texture
// todo2: implement in a game engine w a test program

#include "course/sampler/ISampler.hpp"
#include "course/sampler/SimplexNoiseSampler.hpp"

namespace course {
  namespace sampler {
    namespace terrain {
      class SimpleCourseTerrainSampler : public ISampler<float> {
      public:
        SimpleCourseTerrainSampler();
        SimpleCourseTerrainSampler(const glm::dvec2& origin);
        float Sample(float x, float y) const override;
      private:
        // put a bunch of different resolution samplers here :3

        // course details - little perturbations in slope
        SimplexNoiseSampler high_frequency_course_sampler;

        // course layout - larger perturbations affecting course layout
        SimplexNoiseSampler mid_frequency_course_sampler;

        // sweeping topological features which may affect course layout
        SimplexNoiseSampler low_frequency_course_sampler;

        // broader, larger-amplitude noise which should only become apparent at a distance
        SimplexNoiseSampler infra_frequency_course_sampler;


        glm::dvec2 origin_;
      };
    }
  }
}

#endif // COURSE_TERRAIN_SAMPLER_H_