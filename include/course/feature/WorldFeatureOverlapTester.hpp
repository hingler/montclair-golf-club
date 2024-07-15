#ifndef MGC_WORLD_FEATURE_OVERLAP_TESTER_H_
#define MGC_WORLD_FEATURE_OVERLAP_TESTER_H_

#include "course/feature/WorldFeature.hpp"
#include <memory>

namespace mgc {
  namespace mgc_course {
    // fwd decl
    class WorldFeatureSampler;

    class WorldFeatureOverlapTester {
     public:
      WorldFeatureOverlapTester(
        const std::vector<std::shared_ptr<WorldFeatureSampler>>& samplers
      );

      // tests a given feature
      bool Test(
        const std::shared_ptr<WorldFeature>& test_feature
      ) const;

     private:
      double GetUnderlyingOcclusionSample(double x, double y) const;
      std::vector<std::shared_ptr<WorldFeatureSampler>> samplers;
    };
  }
}

#endif // MGC_WORLD_FEATURE_OVERLAP_TESTER_H_
