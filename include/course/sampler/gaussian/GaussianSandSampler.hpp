#ifndef GAUSSIAN_SAND_SAMPLER_H_
#define GAUSSIAN_SAND_SAMPLER_H_

#include "course/sampler/GaussianMetaballSampler.hpp"
#include "course/path/CoursePath.hpp"
#include "course/path/CompoundCurve.hpp"

#include "course/sampler/gaussian/GaussianConfig.hpp"

#include <glm/glm.hpp>

#include <random>

namespace course {
  namespace sampler {
    namespace gaussian {
      /**
       * @brief Distributes metaball samples to create sand patches
       * 
       */
      class GaussianSandSampler : public GaussianMetaballSampler {
       public:
        GaussianSandSampler(
          std::shared_ptr<GaussianMetaballSampler> fairway_sampler,
          const path::CoursePath& course_path,
          const path::CompoundCurve& bezier_curve
        );

        // (tba: need to build around where i think the green will be for now)
        void Generate(const GaussianPathConfig& config);
       private:
        // combines gradient/hazard samplers
        glm::vec2 GetNetGradient(const glm::vec2& sample_point);
        double GetNetSample(const glm::vec2& sample_point);

        std::shared_ptr<GaussianMetaballSampler> fairway_sampler;
        path::CoursePath path;
        path::CompoundCurve curve;

        // provide engine if we can
        std::mt19937_64 engine;
      };
    }
  }
}

#endif // GAUSSIAN_SAND_SAMPLER_H_