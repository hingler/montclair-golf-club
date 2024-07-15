#include "course/feature/pre/ClubhousePositioner.hpp"
#include "course/feature/FeatureSpec.hpp"
#include "glm/fwd.hpp"
#include "seed/SeedPositioner.hpp"

namespace mgc {
  namespace mgc_course {
    namespace prefeature {
      ClubhousePositioner::ClubhousePositioner(
        const std::shared_ptr<seed::SeedPositioner>& positioner
      ) : positioner(positioner) {}

      std::vector<FeatureSpec> ClubhousePositioner::GetPoints(
        const glm::dvec2& origin,
        const glm::dvec2& size
      ) const {
        std::vector<glm::ivec2> indices;
        std::vector<glm::dvec2> points = positioner->GetSeeds(origin, size, &indices);
        std::vector<FeatureSpec> results;

        assert(indices.size() == points.size());
        for (int i = 0; i < points.size(); i++) {
          const glm::dvec2& point = points.at(i);
          const glm::ivec2& index = indices.at(i);
          FeatureSpec house;
          house.position = point;
          house.rotation_degrees = 0.0;
          house.scale = 1.0;

          // ignore sign bits - x & y bytes printed out
          size_t instance_id =
            static_cast<size_t>(index.x) << 32 | static_cast<size_t>(index.y) & 0x00000000FFFFFFFFUL;

          house.runtime_id = instance_id;

          results.push_back(house);
        }

        return results;
      }
    }
  }
}
