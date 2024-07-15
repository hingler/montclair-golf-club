#ifndef TEE_MARKER_BUILDER_H_
#define TEE_MARKER_BUILDER_H_

#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/generic/BlankWorldFeature.hpp"
#include "mgc/info/HoleInfoFetcher.hpp"
namespace mgc {
  namespace feature {
    namespace post {
      class TeeMarkerBuilder : public mgc_course::WorldFeatureBuilder {
        constexpr static glm::dvec2 TEE_SIZE = glm::dvec2(2.0);
       public:
        TeeMarkerBuilder() {}

        std::vector<std::unique_ptr<mgc_course::WorldFeature>> BuildFeatures(
          const std::shared_ptr<mgc_course::UnderlyingSamplerHeight>& underlying_features,
          const std::vector<mgc_course::FeatureSpec>& points
        ) const override {
          std::vector<std::unique_ptr<mgc_course::WorldFeature>> features;
          for (const auto& point : points) {
            features.push_back(
              std::make_unique<generic::BlankWorldFeature>(
                point.GetOrigin(TEE_SIZE),
                TEE_SIZE,
                0.0,
                1.0
              )
            );
          }

          return features;
        }

        glm::dvec2 GetMaxFeatureSize() const override {
          return TEE_SIZE * 2.0;
        }
       private:
        std::unique_ptr<mgc_course::WorldFeature> BuildFeature(
          const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& underlying_features,
          mgc_course::FeatureSpec& point
        ) const;
      };
    }
  }
}

#endif // TEE_MARKER_BUILDER_H_
