#include "sdf/Bundle.hpp"
#include "sdf_jak/sdf_bundle.h"

namespace mgc {
  Bundle::Bundle(double k) : bundle(bundle_get(k)) {}
  void Bundle::AddCircle(double x, double y, double radius) {
    bundle_add_circle(bundle, x, y, radius);
  }

  void Bundle::AddCapsule(const glm::dvec2* elements, size_t count, double radius) {
    bundle_add_capsule(bundle, reinterpret_cast<const double*>(elements), count, radius);
  }

  double Bundle::Sample(double x, double y) {
    return bundle_dist(bundle, x, y);
  }

  Bundle::Bundle(Bundle&& other) {
    bundle = other.bundle;
    other.bundle = nullptr;
  }

  Bundle& Bundle::operator=(Bundle&& other) {
    bundle = other.bundle;
    other.bundle = nullptr;
    return *this;
  }

  Bundle::~Bundle() {
    if (bundle != nullptr) {
      bundle_free(bundle);
    }
  }
}
