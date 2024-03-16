#include "util/LineUtil.hpp"

namespace mgc {
  namespace util {
    double length_squared(const glm::dvec2& line) {
      return line.x * line.x + line.y * line.y;
    }

    // squared dist!!!!!!!!!!!!!!!!!!!!!!
    double dist_to_line(const glm::dvec2& sample, const glm::dvec2& start, const glm::dvec2& end) {
      // project dist to point onto line, subtract projected component
      // if projection is not on the line, then use dist to point
      glm::dvec2 e_dist = end - sample;

      glm::dvec2 line_proj = glm::normalize(end - start);

      // perpendicular point in global space
      glm::dvec2 perp_point = end - (glm::dot(e_dist, line_proj) * line_proj);

      glm::dvec2 bb_min(glm::min(start.x, end.x), glm::min(start.y, end.y));
      glm::dvec2 bb_max(glm::max(start.x, end.x), glm::max(start.y, end.y));

      // perp point is outside bounding box ie not on line - use line dist
      if (
          perp_point.x < bb_min.x
      ||  perp_point.y < bb_min.y
      ||  perp_point.x > bb_max.x
      ||  perp_point.y > bb_max.y
      ) {
        glm::dvec2 s_dist = start - sample;

        return glm::min(
          length_squared(s_dist),
          length_squared(e_dist)
        );
      } else {
        return length_squared(sample - perp_point);
      }
    }
  }
}
