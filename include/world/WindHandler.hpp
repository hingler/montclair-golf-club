#ifndef MGC_WIND_HANDLER_H_
#define MGC_WIND_HANDLER_H_

#include "glm/fwd.hpp"
#include <glm/glm.hpp>

namespace mgc {
  namespace world {
    struct WindHandler {
      // returns wind speed at a given point
      virtual glm::dvec3 GetWind(const glm::dvec3& position) const = 0;
      // https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
      // need to swap out damping for "real" drag force
    };
  }
}

#endif // MGC_WIND_HANDLER_H_
