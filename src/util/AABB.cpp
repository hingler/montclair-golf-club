#include "util/GC_AABB.hpp"


namespace mgc {
  GC_AABB::GC_AABB() {}
  GC_AABB::GC_AABB(const std::vector<glm::dvec2>& points) {
    glm::dvec2 min(std::numeric_limits<double>::max());
    glm::dvec2 max(std::numeric_limits<double>::lowest());

    for (const auto& p : points) {
      min = glm::min(min, p);
      max = glm::max(max, p);
    }

    origin = min;
    size = (max - min);
  }

  GC_AABB::GC_AABB(const glm::dvec2& origin, const glm::dvec2& size) : origin(origin), size(size) {}

  // hate this immensely
  GC_AABB GC_AABB::merge(const GC_AABB& other) {
    glm::dvec2 start = glm::min(origin, other.origin);
    glm::dvec2 end = glm::max(origin + size, other.origin + other.size);
    return GC_AABB {
      start,
      (end - start)
    };
  }
}
