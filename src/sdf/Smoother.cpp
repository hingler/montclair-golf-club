// present scons file will work for this

#include "sdf/Smoother.hpp"
#include "sdf_jak/sdf_bundle.h"

namespace mgc {
  Smoother::Smoother(Bundle& a, Bundle& b, double factor)
    : smoother(smoother_get(a.raw_ptr(), b.raw_ptr(), factor)) {}

  double Smoother::Sample(double x, double y) {
    return smoother_dist(smoother, x, y);
  }

  Smoother::~Smoother() {
    if (smoother != nullptr) {
      smoother_free(smoother);
    }
  }

  Smoother::Smoother(Smoother&& other) {
    smoother = other.smoother;
    other.smoother = nullptr;
  }

  Smoother& Smoother::operator=(Smoother&& other) {
    smoother = other.smoother;
    other.smoother = nullptr;
    return *this;
  }
}
