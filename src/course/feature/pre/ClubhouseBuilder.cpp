#include "course/feature/pre/ClubhouseBuilder.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/generic/BlankWorldFeature.hpp"
#include "course/sampler/ConstSampler.hpp"
#include "course/sampler/ZeroSampler.hpp"
#include "gog43/Logger.hpp"
#include "sdf/RectSDF.hpp"

#include "course/feature/height/UnderlyingSamplerHeight.hpp"

namespace mgc {
  namespace mgc_course {
    namespace prefeature {
      ClubhouseBuilder::ClubhouseBuilder() {}

      std::vector<std::unique_ptr<WorldFeature>> ClubhouseBuilder::BuildFeatures(
        const std::shared_ptr<UnderlyingSamplerHeight>& underlying_features,
        const std::vector<FeatureSpec>& points
      ) const {
        std::vector<std::unique_ptr<WorldFeature>> results;

        for (const auto& point : points) {
          cg::FeatureBox box(
            point.position - (MAX_SIZE),
            2.0 * MAX_SIZE
          );

          // need to do it like this hehe
          auto rect = std::make_shared<RectSDF>(
            MAX_SIZE,
            glm::dvec2(16.0),
            0.0
          );

          auto zero = std::make_shared<sampler::ZeroSampler>();

          auto smoother = std::make_shared<cg::smooth::LocalizedSmoother<UnderlyingSamplerHeight, RectSDF>>(
            underlying_features,
            box.GetOrigin()
          );

          // tba - this is working thoguh

          smoother->AddSubSmoother(
            rect,
            MAX_SIZE - 16.0,
            MAX_SIZE + 16.0,
            48.0,
            0.008
          );

          // std::unique_ptr<WorldFeature> feature = std::make_unique<feature::generic::BlankWorldFeature>(
          //   box.GetOrigin(), box.GetSize(), 0.0, 1.0
          // );

          // 1.0 on const sampler could be a problem??

          std::unique_ptr<WorldFeature> feature = std::make_unique<WorldFeatureImpl<
            UnderlyingSamplerHeight,
            RectSDF,
            sampler::ConstSampler
          >>(
            box,
            zero,
            zero,
            zero,
            zero,
            smoother,
            std::make_shared<sampler::ConstSampler>(1.0),
            0.0,
            1.0
          );

          // course layout should change (need to check)
          // try dropping this, see if clubhouse is somehow the problem
          results.push_back(std::move(feature));
        }

        if (results.size() > 0) {
          gog43::print("CLUBHOUSE: created ", results.size(), " clubhouses");
        }

        return results;
      }

      glm::dvec2 ClubhouseBuilder::GetMaxFeatureSize() const {
        return MAX_SIZE;
      }
    }
  }
}
