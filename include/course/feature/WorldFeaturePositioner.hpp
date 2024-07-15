#ifndef MGC_WORLD_FEATURE_POSITIONER_H_
#define MGC_WORLD_FEATURE_POSITIONER_H_

#include "course/feature/FeatureSpec.hpp"
#include <vector>
namespace mgc {
  namespace mgc_course {
    class WorldFeaturePositioner {
     public:
      // given some region of space, fetch all potential samplers
      virtual std::vector<FeatureSpec> GetPoints(
        const glm::dvec2& origin,
        const glm::dvec2& size
      ) const = 0;
    };
  }
}

#endif // MGC_WORLD_FEATURE_POSITIONER_H_
