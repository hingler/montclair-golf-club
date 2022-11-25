#ifndef CIRCLE_INTERSECTION_TESTER_H_
#define CIRCLE_INTERSECTION_TESTER_H_

#include <glm/glm.hpp>

namespace course {
  namespace generator {
    namespace impl {
      float GetDistanceToCircle(const glm::vec2& point, const glm::vec2& direction, const glm::vec2& circle_center, float circle_radius);
    }
  }
}

#endif // CIRCLE_INTERSECTION_TESTER_H_