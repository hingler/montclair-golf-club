#ifndef DRAW_CANVAS_H_
#define DRAW_CANVAS_H_

#include <glm/glm.hpp>

namespace mgc {
  namespace impl {
    class DrawCanvas {
      // this is all i want
     public:
      virtual glm::dvec4 Sample(double x, double y) const = 0;
    };
  }
}

#endif // DRAW_CANVAS_H_