#ifndef PATH_NODE_H_
#define PATH_NODE_H_

// paths are never joined to branches
// when we create a branch, we just terminate the path, add a branch node, and return two paths

#include <glm/glm.hpp>

#include <vector>

// how to store paths?
// - does a path node point to the next branch node?


namespace mgc {
  struct BranchNode;

  struct PathNode {
    /// Creates a new path node from a series of points
    // nothing to it really
    PathNode(const glm::dvec2& start) { points.push_back(start); }

    const glm::dvec2& operator[](size_t i) const { return points[i]; }

    // ibnar
    const glm::dvec2& cat(size_t i) const { return points[i]; }
    glm::dvec2& at(size_t i) { return points[i]; }

    glm::dvec2 get(size_t i) const { return points[i]; }

    size_t size() const { return points.size(); }
    void append(const glm::dvec2& point) { points.push_back(point); }
    BranchNode* next = nullptr;
   private:
    std::vector<glm::dvec2> points;
  };
}

#endif // PATH_NODE_H_
