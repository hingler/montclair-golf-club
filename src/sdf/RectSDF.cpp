#include "sdf/RectSDF.hpp"
#include <algorithm>
#include <cmath>


namespace mgc {
  RectSDF::RectSDF(
    const glm::dvec2& center,
    const glm::dvec2& dimensions,
    double rotation_degrees
  ) {
    glm::dvec2 dims = dimensions;
    double degs = rotation_degrees;

    double rads = glm::radians(degs);

    glm::dvec2 cross_axis(
      std::cos(rads),
      std::sin(rads)
    );

    // dims x is bigger than dims y
    thickness = dims.y / 2.0;
    double pt_dist = dims.x / 2.0;
    a = -cross_axis * pt_dist + center;
    b = cross_axis * pt_dist + center;

    l = glm::length(b - a);
    if (l > 0.0001) {
      d = glm::normalize(b - a);
    } else {
      // possible bug in normalization here - square sampler hehe
      d = glm::dvec2(1.0, 0.0);
    }
  }

  double RectSDF::Sample(double x, double y) const {
    glm::dvec2 p(x, y);
    glm::dvec2 q = (p - (a + b) * 0.5);
    q = glm::mat2(d.x, -d.y, d.y, d.x) * q;
    q = glm::abs(q) - glm::dvec2(l * 0.5, thickness);
    return glm::length(glm::max(q, 0.0)) + glm::min(glm::max(q.x, q.y), 0.0);

  }
}
