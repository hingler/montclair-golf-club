#include "course/feature/gen/FeatureInstanceSpawner.hpp"
#include "course/feature/FeatureSpec.hpp"

namespace mgc {
  namespace mgc_course {
    namespace mgc_gen {
      FeatureInstanceSpawner::FeatureInstanceSpawner(
        const std::shared_ptr<WorldFeaturePositioner>& positioner,
        const std::shared_ptr<WorldFeatureInstancer>& instancer
      ) : positioner(positioner), instancer(instancer) {}

      void FeatureInstanceSpawner::Update(
        const glm::dvec3& position,
        double max_dist
      ) {
        glm::dvec2 origin = glm::dvec2(position.x, position.z) - max_dist;
        glm::dvec2 size(2.0 * max_dist);

        std::vector<FeatureSpec> points = positioner->GetPoints(origin, size);
        instancer->UpdateInstances(
          points
        );
      }

      void FeatureInstanceSpawner::Clear() {
        // instantiate with 0 features
        instancer->UpdateInstances(
          std::vector<FeatureSpec>()
        );
      }
    }
  }
}
