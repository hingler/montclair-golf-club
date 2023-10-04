#include "course/path/CoursePath.hpp"

#include <algorithm>

namespace course {
  namespace path {
    void CoursePath::Recenter(const glm::vec2& start, const glm::vec2& end) {
      if (course_path.size() <= 0) {
        return;
      }

      glm::vec2 bb_start(course_path[0]);
      glm::vec2 bb_end(course_path[0]);

      for (auto& position : course_path) {
        bb_start.x = std::min(position.x, bb_start.x);
        bb_start.y = std::min(position.y, bb_start.y);
        bb_end.x = std::max(position.x, bb_end.x);
        bb_end.y = std::max(position.y, bb_end.y);
      }

      glm::vec2 size = end - start;

      auto ideal_start = (size / 2.0f) - ((bb_end - bb_start) / 2.0f) + start;
      for (auto& position : course_path) {
        position += (ideal_start - bb_start);
      }
    }
  }
}