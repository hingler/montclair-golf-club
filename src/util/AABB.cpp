#include "util/AABB.hpp"
#include <limits>


namespace mgc {
  AABB::AABB() {}
  AABB::AABB(const std::vector<glm::dvec2>& points) {
    glm::dvec2 min(std::numeric_limits<double>::max());
    glm::dvec2 max(std::numeric_limits<double>::lowest());

    for (const auto& p : points) {
      min = glm::min(min, p);
      max = glm::max(max, p);
    }

    origin = min;
    size = (max - min);
  }

  AABB AABB::merge(const AABB& other) {
    glm::dvec2 start = glm::min(origin, other.origin);
    glm::dvec2 end = glm::max(origin + size, other.origin + other.size);
    return AABB {
      start,
      (end - start)
    };
  }
}
