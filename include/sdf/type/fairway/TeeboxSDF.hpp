#ifndef TEEBOX_SDF_H_
#define TEEBOX_SDF_H_

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
      // center of teebox
      const glm::dvec2 center;
      // direction which it faces in (fwd)
      const glm::dvec2 direction;
      // width/height (wrt direction)
      const glm::dvec2 dimensions;
    };
  }
}

#endif // TEEBOX_SDF_H_
