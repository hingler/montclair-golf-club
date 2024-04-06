#include "sdf/CPPBundle.hpp"
#include <algorithm>

namespace mgc {
  CPPBundle::CPPBundle() : CPPBundle(0.0) {}
  CPPBundle::CPPBundle(double k) : k(k) {

  }

  void CPPBundle::AddCircle(double x, double y, double radius) {
    circles.push_back(
      glm::dvec3(x, y, radius)
    );
  }

  bool CPPBundle::AddCapsule(
    const std::vector<glm::dvec2>& elements,
    const std::vector<double>& radii
  ) {
    if (elements.size() < 0 || elements.size() != radii.size()) {
      // undefined behavior
      return false;
    }

    // swag
    capsules.push_back(
      CPPCapsule(
        elements,
        radii
      )
    );

    return true;
  }

  void CPPBundle::AddCapsule(
    const std::vector<glm::dvec2>& elements,
    double radius
  ) {
    capsules.push_back(
      CPPCapsule(
        elements,
        radius
      )
    );
  }

  double CPPBundle::Sample(double x, double y) const {
    glm::dvec2 point(x, y);
    double min_dist = std::numeric_limits<double>::max();
    for (auto& circle : circles) {
      min_dist = smin_f(
        min_dist,
        glm::length(point - reinterpret_cast<const glm::dvec2&>(circle)) - circle.z
      );
    }

    for (auto& capsule : capsules) {
      min_dist = smin_f(
        min_dist,
        capsule.Sample(point)
      );
    }

    return min_dist;
  }

  double CPPBundle::Sample(double x, double y, double z) const {
    glm::dvec3 point(x, y, z);
    double min_dist = std::numeric_limits<double>::max();
    glm::dvec3 working_c;
    working_c.z = 0.0;
    for (auto& circle : circles) {
      working_c.x = circle.x;
      working_c.y = circle.y;

      min_dist = smin_f(
        min_dist,
        glm::length(point - working_c) - circle.z
      );
    }

    for (auto& capsule : capsules) {
      min_dist = smin_f(
        min_dist,
        capsule.Sample(point)
      );
    }

    return min_dist;
  }

  double CPPBundle::smin_f(
    double a,
    double b
  ) const {
    if (k < 0.00001) {
      // cost? prob small - easy to predict
      return std::min(a, b);
    }

    double h = std::max(k - std::abs(a - b), 0.0) / k;
    return std::min(a, b) - h * h * k * 0.25;
  }
}
