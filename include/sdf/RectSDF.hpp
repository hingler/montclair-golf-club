#ifndef MGC_RECT_SDF_H_
#define MGC_RECT_SDF_H_

#include <glm/glm.hpp>

namespace mgc {
  class RectSDF {
   public:
    RectSDF(
      const glm::dvec2& center,
      const glm::dvec2& dimensions,
      double rotation_degrees
    );

    double Sample(double x, double y) const;
   private:
    // https://iquilezles.org/articles/distfunctions2d/
    glm::dvec2 a;
    glm::dvec2 b;
    double thickness;

    // cache
    double l;
    glm::dvec2 d;
  };
}

#endif // MGC_RECT_SDF_H_
