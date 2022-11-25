#include "course/generator/impl/GetDistanceToCircle.hpp"

#include <glm/glm.hpp>

namespace course {
  namespace generator {
    namespace impl {
      float GetDistanceToCircle(const glm::vec2& point, const glm::vec2& direction, const glm::vec2& circle_center, float circle_radius) {
        glm::vec2 center_to_point = (point - circle_center);
        float dist_from_closest = glm::dot(center_to_point, direction);
        glm::vec2 closest_point = point - direction * dist_from_closest;
        float closest_dist = glm::length(closest_point - circle_center);
        float theta = acos(closest_dist / circle_radius);
        float dist_to_cover = circle_radius * sin(theta) - dist_from_closest;
        return dist_to_cover;
      }
    }
  }
}