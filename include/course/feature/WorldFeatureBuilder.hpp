#ifndef MGC_WORLD_FEATURE_BUILDER_H_
#define MGC_WORLD_FEATURE_BUILDER_H_

#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeature.hpp"
#include <glm/glm.hpp>
namespace mgc {
  namespace mgc_course {

    // fwd decl underlyingsamplerheight
    class UnderlyingSamplerHeight;

    namespace mgc_gen {
      // fwd decl to make compiler happy :)
      class WorldFeatureManager;
    }

    class WorldFeatureBuilder {
     public:
      // given a series of points, creates and returns some number of world features
      // should we handle rotation here, or should we handle it above?
      // (note: need to pass in tester, probably)

      virtual std::vector<std::unique_ptr<WorldFeature>> BuildFeatures(
        const std::shared_ptr<UnderlyingSamplerHeight>& underlying_features,
        const std::vector<FeatureSpec>& points
      ) const = 0;

      virtual glm::dvec2 GetMaxFeatureSize() const = 0;

      // thinking: arrange features by chunks in chunker
      // we receive a region, we pad out said region based on feature size, then we convert that to chunks
      // store set of processed chunks alongside multisampler (ie avoiding multigen)
    };
  }
}

#endif // MGC_WORLD_FEATURE_BUILDER_H_
