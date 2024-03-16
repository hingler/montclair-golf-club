#ifndef OVERLAP_TESTER_H_
#define OVERLAP_TESTER_H_

#include <glm/glm.hpp>

namespace mgc {
  // checks if we're overlapping with a specific feature
  class OverlapTester {
   public:
    /// Return true if a path overlaps with some feature - false otherwise.
    virtual bool Test(const glm::dvec2& point) const = 0;
    virtual ~OverlapTester() {}
  };
}

#endif // OVERLAP_TESTER_H_
