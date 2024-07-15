#ifndef TEEBOX_SDF_H_
#define TEEBOX_SDF_H_

#include "sdf/RectSDF.hpp"
#include <glm/glm.hpp>

namespace mgc {
  namespace fairway {
    class TeeboxSDF {
     public:
      TeeboxSDF(
        const glm::dvec2& center,
        const glm::dvec2& direction,
        const glm::dvec2& dimensions
      );

      double Sample(double x, double y) const;
     private:

      RectSDF delegate;
    };
  }
}

#endif // TEEBOX_SDF_H_
