#include "course/feature/gen/FeatureSubSampler.hpp"
#include "course/feature/WorldFeature.hpp"
#include <algorithm>

namespace mgc {
  namespace mgc_course {
    namespace mgc_gen {
      FeatureSubSampler::FeatureSubSampler(
        const std::vector<std::shared_ptr<const WorldFeature>>& features
      ) : features(features) {}

      double FeatureSubSampler::SampleHeight(double x, double y, double underlying) const {
        return features.SampleHeight(x, y, underlying);
      }

      glm::vec4 FeatureSubSampler::SampleSplat(double x, double y, size_t index) const {
        return features.SampleSplat(x, y, index);
      }

      float FeatureSubSampler::SampleTreeFill(double x, double y) const {
        return features.SampleTreeFill(x, y);
      }

      float FeatureSubSampler::SampleGrassFill(double x, double y) const {
        return features.SampleGrassFill(x, y);
      }

      double FeatureSubSampler::SampleOcclusion(double x, double y) const {
        auto itr = features.cbegin();
        double acc = std::numeric_limits<double>::max();
        while (itr != features.cend()) {
          acc = std::min(acc, (*itr++)->SampleOcclusion(x, y));
        }

        return acc;
      }
    }
  }
}
