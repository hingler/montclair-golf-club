#ifndef MGC_CLUBHOUSE_BUILDER_H_
#define MGC_CLUBHOUSE_BUILDER_H_

#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"

namespace mgc {
  namespace mgc_course {
    namespace prefeature {
      class ClubhouseBuilder : public WorldFeatureBuilder {
       public:
        ClubhouseBuilder();
        std::vector<std::unique_ptr<WorldFeature>> BuildFeatures(
          const std::shared_ptr<UnderlyingSamplerHeight>& underlying_features,
          const std::vector<FeatureSpec>& points
        ) const override;

        glm::dvec2 GetMaxFeatureSize() const override;
       private:
        static constexpr glm::dvec2 MAX_SIZE = glm::dvec2(96.0);
      };
    }
  }
}

#endif // MGC_CLUBHOUSE_BUILDER_H_
