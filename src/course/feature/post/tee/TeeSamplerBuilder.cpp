#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureInstancer.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "course/feature/post/tee/TeeMarkerBuilder.hpp"
#include "course/feature/post/tee/TeeMarkerPositioner.hpp"
#include "course/feature/post/tee/TeeSamplerBuilder.hpp"
#include "mgc/info/HoleInfoFetcher.hpp"

using mgc::mgc_course::WorldFeatureBuilder;
using mgc::mgc_course::WorldFeaturePositioner;

namespace mgc {
  namespace feature {
    namespace post {
      TeeSamplerBuilder::TeeSamplerBuilder(
        const std::shared_ptr<HoleInfoFetcher>& fetcher
      ) : builder(std::make_shared<TeeMarkerBuilder>()),
      positioner(std::make_shared<TeeMarkerPositioner>(fetcher)) {}

      std::shared_ptr<mgc_course::WorldFeaturePositioner> TeeSamplerBuilder::GetPositioner() const {
        return positioner;
      }

      std::shared_ptr<mgc_course::WorldFeatureBuilder> TeeSamplerBuilder::GetBuilder() const {
        return builder;
      }
    }
  }
}
