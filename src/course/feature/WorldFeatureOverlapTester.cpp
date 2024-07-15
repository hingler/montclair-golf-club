#include "course/feature/WorldFeatureOverlapTester.hpp"
#include "course/feature/WorldFeatureSampler.hpp"

namespace mgc {
  namespace mgc_course {
    WorldFeatureOverlapTester::WorldFeatureOverlapTester(
      const std::vector<std::shared_ptr<WorldFeatureSampler>>& samplers
    ) : samplers(samplers) {}

    bool WorldFeatureOverlapTester::Test(
      const std::shared_ptr<WorldFeature>& test_feature
    ) const {
      glm::dvec2 pt = test_feature->GetFeatureCenter();
      double intersect_dist = std::max(
        test_feature->SampleOcclusion(pt.x, pt.y),
        GetUnderlyingOcclusionSample(pt.x, pt.y)
      );

      glm::dvec2 feature_size = test_feature->GetSize();
      // length of diag / 2, plus epsilon of 0.5
      double threshold_dist = (glm::length(feature_size) + 1.0) / 2.0;
      if (threshold_dist < intersect_dist) {
        // must march at least the diag of our box to find occluded region - no intersect
        return false;
      } else {
        // simple contain test i'm thinking
        for (const auto& sampler : samplers) {
          WorldFeatureSampler::output_type boxes = sampler->GetFeatures(
            test_feature->GetOrigin(),
            feature_size
          );

          if (boxes.size() > 0) {
            // at least one box overlaps - let's just return true for now
            return true;
          }
        }

        return false;
      }

    }

    double WorldFeatureOverlapTester::GetUnderlyingOcclusionSample(double x, double y) const {
      double acc = std::numeric_limits<double>::max();
      for (const auto& sampler : samplers) {
        acc = std::min(acc, sampler->SampleOcclusion(x, y));
      }

      return acc;
    }
  }
}
