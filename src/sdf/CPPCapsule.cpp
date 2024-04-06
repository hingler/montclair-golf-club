#include "sdf/CPPCapsule.hpp"

#include <cmath>
#include <cstdlib>
#include <limits>

namespace mgc {
  CPPCapsule::CPPCapsule() {}

  CPPCapsule::CPPCapsule(
    const std::vector<glm::dvec2>& elements,
    double radius
  ) : CPPCapsule(
    elements,
    std::vector<double>(elements.size(), radius)
  ) {}

  CPPCapsule::CPPCapsule(
    const std::vector<glm::dvec2>& elements,
    const std::vector<double>& radii
  ) : elements(elements), radii(radii) {}

  double CPPCapsule::Sample(double x, double y) const {
    return Sample(glm::dvec2(x, y));
  }

  double CPPCapsule::Sample(const glm::dvec2& point) const {
    double min_dist = std::numeric_limits<double>::max();
    for (size_t i = 1; i < elements.size(); i++) {
      const glm::dvec2& a = elements.at(i - 1);
      const glm::dvec2& b = elements.at(i);

      const double& ra = radii.at(i - 1);
      const double& rb = radii.at(i);

      min_dist = std::min(
        min_dist,
        CPPCapsule::SampleCapsule(
          point,
          a,
          ra,
          b,
          rb
        )
      );
    }

    return min_dist;
  }

  double CPPCapsule::Sample(const glm::dvec3& point) const {
    double min_dist = std::numeric_limits<double>::max();

    glm::dvec3 working_a, working_b;

    working_a.z = 0;
    working_b.z = 0;

    for (size_t i = 1; i < elements.size(); i++) {
      const glm::dvec2& pa = elements.at(i - 1);
      const glm::dvec2& pb = elements.at(i);

      const double& ra = radii.at(i - 1);
      const double& rb = radii.at(i);

      working_a.x = pa.x;
      working_a.y = pa.y;
      working_b.x = pb.x;
      working_b.y = pb.y;

      min_dist = std::min(
        min_dist,
        CPPCapsule::SampleCapsule3D(
          point,
          working_a,
          ra,
          working_b,
          rb
        )
      );
    }

    return min_dist;
  }

  double CPPCapsule::SampleCapsule(
    const glm::dvec2& point,
    const glm::dvec2& point_a,
    const double& radius_a,
    const glm::dvec2& point_b,
    const double& radius_b
  ) {
    glm::dvec2 p = (point - point_a);
    glm::dvec2 pb = (point_b - point_a);

    // how to do it?
    // - tx is the same

    double h = glm::dot(pb, pb);
    glm::dvec2 q(
      glm::dot(p, glm::dvec2(pb.y, -pb.x)) / h,
      glm::dot(p, pb) / h
    );

    q.x = std::abs(q.x);

    double b = (radius_a - radius_b);
    glm::dvec2 c(
      glm::sqrt(std::max(h - (b * b), 0.00001)),
      b
    );

    double k = c.x * q.y - c.y * q.x;
    double m = glm::dot(c, q);
    double n = glm::dot(q, q);

    if (k < 0.0) {
      return std::sqrt(h * n) - radius_a;
    } else if (k > c.x) {
      return std::sqrt(h * (n + 1.0 - 2.0 * q.y)) - radius_b;
    }

    return m - radius_a;
  }

  double CPPCapsule::SampleCapsule3D(
    const glm::dvec3& point,
    const glm::dvec3& a,
    const double& r1,
    const glm::dvec3& b,
    const double& r2
  ) {
    // https://iquilezles.org/articles/distfunctions/
    glm::dvec3 ba = b - a;
    double l2 = glm::dot(ba, ba);
    double rr = r1 - r2;
    double a2 = l2 - (rr * rr);
    double il2 = (1.0 / l2);

    glm::dvec3 pa = (b - a);
    double y = glm::dot(pa, ba);
    double z = y - l2;
    glm::dvec3 x2_inter = (pa * l2 - ba * y);
    double x2 = glm::dot(x2_inter, x2_inter);
    double y2 = y * y * l2;
    double z2 = z * z * l2;

    double k = glm::sign(rr) * rr * rr * x2;
    if ( glm::sign(z) * a2 * z2 > k ) return glm::sqrt(x2 + z2) * il2 - r2;
    if ( glm::sign(y) * a2 * y2 < k ) return glm::sqrt(x2 + y2) * il2 - r1;

    return (glm::sqrt(x2 * a2 * il2) + y * rr) * il2 - r1;
  }
}
