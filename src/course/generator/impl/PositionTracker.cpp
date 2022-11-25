#include "course/generator/impl/PositionTracker.hpp"

namespace course {
  namespace generator {
    namespace impl {
      PositionTracker::PositionTracker() : shots(0) {}

      void PositionTracker::PushPosition(const glm::vec2& pos) {
        if (shots < 6) {
          positions[shots++] = pos;
        }
      }

      glm::vec2 PositionTracker::PopPosition() {
        shots--;
        if (shots < 0) shots = 0;

        return positions[shots];
      }
    }
  }
}