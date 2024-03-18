#include "sdf/type/sand/BaseSandSDF.hpp"

namespace mgc {
  namespace sand {
    BaseSandSDF::BaseSandSDF(const std::shared_ptr<CPPBundle>& traps) : traps(traps) {}

    double BaseSandSDF::Sample(double x, double y) const {
      return traps->Sample(x, y);
    }

    std::shared_ptr<CPPBundle> BaseSandSDF::GetBundle() {
      return traps;
    }
  }
}
