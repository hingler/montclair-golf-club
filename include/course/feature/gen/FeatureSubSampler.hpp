#ifndef FEATURE_SUB_SAMPLER_H_
#define FEATURE_SUB_SAMPLER_H_

#include "corrugate/sampler/SmoothingMultiBoxSampler.hpp"
#include "course/feature/WorldFeature.hpp"
#include <glm/glm.hpp>
namespace mgc {
  namespace mgc_course {
    namespace mgc_gen {
      class FeatureSubSampler {
       public:
        FeatureSubSampler(
          const std::vector<std::shared_ptr<const WorldFeature>>& features
        );

        double SampleHeight(double x, double y, double underlying) const;
        glm::vec4 SampleSplat(double x, double y, size_t index) const;
        float SampleTreeFill(double x, double y) const;
        float SampleGrassFill(double x, double y) const;
        double SampleOcclusion(double x, double y) const;
       private:
        cg::SmoothingMultiBoxSampler<WorldFeature> features;
      };
    }
  }
}

#endif // FEATURE_SUB_SAMPLER_H_
