#ifndef TEE_MARKER_POSITIONER_H_
#define TEE_MARKER_POSITIONER_H_

#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "mgc/info/HoleInfoFetcher.hpp"
#include "seed/hole/ConversionResult.hpp"
#include <cmath>
namespace mgc {
  namespace feature {
    namespace post {
      class TeeMarkerPositioner : public mgc_course::WorldFeaturePositioner {
       public:
        TeeMarkerPositioner(
          const std::shared_ptr<HoleInfoFetcher>& fetcher,
          const double tee_width = 16.0
        ) : fetcher(fetcher), tee_width(tee_width) {}

        std::vector<mgc_course::FeatureSpec> GetPoints(
          const glm::dvec2& origin,
          const glm::dvec2& size
        ) const override {
          std::vector<mgc_course::FeatureSpec> result;

          std::vector<HoleInfo> info = fetcher->GetHoleInfo(origin, size);
          const double marker_dist = (tee_width - 2.0) / 2.0;

          for (const auto& hole : info) {
            glm::dvec2 tee = hole.GetTeePosition();
            cg::FeatureBox test_box(origin, size);
            if (test_box.Contains(tee)) {
              glm::dvec2 tee_cross = glm::rotate(hole.GetTeeDirection(), M_PI / 2.0);

              // create tee markers
              {
                mgc_course::FeatureSpec marker_near, marker_far;
                marker_near.position = tee - tee_cross * marker_dist;
                marker_far.position = tee + tee_cross * marker_dist;

                marker_near.rotation_degrees = 0.0;
                marker_far.rotation_degrees = 0.0;

                marker_near.scale = 0.25;
                marker_far.scale = 0.25;

                marker_near.runtime_id = hole.data.runtime_id * 2;
                marker_far.runtime_id = hole.data.runtime_id * 2 + 1;

                result.push_back(marker_near);
                result.push_back(marker_far);
              }
            }
          }

          return result;
        }

       private:
        std::shared_ptr<HoleInfoFetcher> fetcher;
        const double tee_width;
      };
    }
  }
}

#endif // TEE_MARKER_POSITIONER_H_
