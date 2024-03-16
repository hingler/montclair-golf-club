#ifndef LINE_UTIL_H_
#define LINE_UTIL_H_

#include <glm/glm.hpp>

namespace mgc {
  namespace util {
    double length_squared(const glm::dvec2& line);
    double dist_to_line(const glm::dvec2& sample, const glm::dvec2& start, const glm::dvec2& end);
  }
}

#endif // LINE_UTIL_H_
