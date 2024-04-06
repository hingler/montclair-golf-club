#ifndef MGC_AABB_H_
#define MGC_AABB_H_

#include <glm/glm.hpp>

#include <vector>

// need to homologate these??

namespace mgc {
  struct GC_AABB {
    glm::dvec2 origin;
    glm::dvec2 size;

    // create AABB from a set of point s
    GC_AABB();
    GC_AABB(const std::vector<glm::dvec2>& points);
    GC_AABB(const glm::dvec2& origin, const glm::dvec2& size);

    // returns an AABB which merges this AABB with another
    GC_AABB merge(const GC_AABB& other);
  };
}

#endif // MGC_AABB_H_
