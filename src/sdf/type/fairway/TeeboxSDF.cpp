#include "sdf/type/fairway/TeeboxSDF.hpp"

#include <gog43/Logger.hpp>

namespace mgc {
  namespace fairway {
    TeeboxSDF::TeeboxSDF(
      const glm::dvec2& center,
      const glm::dvec2& direction,
      const glm::dvec2& dimensions
    ) : center(center), direction(glm::normalize(direction)), dimensions(dimensions) {}

    double TeeboxSDF::Sample(double x, double y) const {
      glm::dvec2 point(x, y);
      glm::dvec2 center_relative = point - center;
      double axis_main = glm::abs(glm::dot(direction, center_relative));
      // always positive
      double axis_cross = glm::length(center_relative - (direction * axis_main));

      glm::dvec2 dist_local = glm::dvec2(axis_cross, axis_main) - dimensions;


      // note: prune redundant points from point list?
      // https://iquilezles.org/articles/distfunctions2d/ again lole
      // dist external + -(dist from edge internal)
      double res = glm::length(glm::max(dist_local, glm::dvec2(0.0))) + glm::min(glm::max(dist_local.x, dist_local.y), 0.0);

      return res;
    }
  }
}
