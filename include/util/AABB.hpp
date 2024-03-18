#ifndef AABB_H_
#define AABB_H_

#include <glm/glm.hpp>

#include <vector>

namespace mgc {
  struct AABB {
    glm::dvec2 origin;
    glm::dvec2 size;

    // create AABB from a set of point s
    AABB();
    AABB(const std::vector<glm::dvec2>& points);
    AABB(const glm::dvec2& origin, const glm::dvec2& size);

    // returns an AABB which merges this AABB with another
    AABB merge(const AABB& other);
  };
}

#endif // AABB_H_
