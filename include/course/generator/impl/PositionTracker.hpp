#ifndef POSITION_TRACKER_H_
#define POSITION_TRACKER_H_

#include <glm/glm.hpp>

namespace course {
  namespace generator {
    namespace impl {
      struct PositionTracker {

      public:
        PositionTracker();
        void PushPosition(const glm::vec2& pos);
        glm::vec2 PopPosition();

        glm::vec2 positions[6];
        int shots;
      };
    }
  }
}

#endif // POSITION_TRACKER_H_