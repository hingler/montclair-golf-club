#ifndef RECT_H_
#define RECT_H_

#include <glm/glm.hpp>

namespace course {
  namespace path {
    struct Rect {
      // most negative coordinate
      glm::vec2 start;

      // most positive coordinate
      glm::vec2 end;
    };
  }
}

#endif // RECT_H_