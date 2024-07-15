#ifndef MGC_TEE_SAMPLER_BUILDER_H_
#define MGC_TEE_SAMPLER_BUILDER_H_

#include "course/feature/SamplerBuilder.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureInstancer.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "course/feature/post/tee/TeeMarkerBuilder.hpp"
#include "course/feature/post/tee/TeeMarkerPositioner.hpp"


#include <memory>

namespace mgc {
  namespace feature {
    namespace post {
      class TeeSamplerBuilder : public mgc_course::SamplerBuilder {
       public:
        TeeSamplerBuilder(
          const std::shared_ptr<HoleInfoFetcher>& fetcher
        );

        std::shared_ptr<mgc_course::WorldFeaturePositioner> GetPositioner() const override;
        std::shared_ptr<mgc_course::WorldFeatureBuilder> GetBuilder() const override;

       private:
        std::shared_ptr<TeeMarkerBuilder> builder;
        std::shared_ptr<TeeMarkerPositioner> positioner;
      };
    }
  }
}

#endif // MGC_TEE_SAMPLER_BUILDER_H_
