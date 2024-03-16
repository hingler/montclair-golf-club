#include "sdf/type/fairway/BaseFairwaySDF.hpp"
#include <algorithm>

namespace mgc {
  namespace fairway {
    BaseFairwaySDF::BaseFairwaySDF(
      const std::shared_ptr<Bundle>& bundle,
      const glm::dvec2& center,
      const glm::dvec2& direction,
      const glm::dvec2& dimensions
    ) : bundle(bundle), teebox(std::make_shared<TeeboxSDF>(center, direction, dimensions)) {}

    double BaseFairwaySDF::Sample(double x, double y) const {
      // make it an sdf!!!
      double bundle_sample = bundle->Sample(x, y);
      double teebox_sample = teebox->Sample(x, y);

      return std::min(bundle_sample, teebox_sample);
    }

    std::shared_ptr<Bundle> BaseFairwaySDF::GetBundle() {
      return bundle;
    }

    std::shared_ptr<const TeeboxSDF> BaseFairwaySDF::GetTeebox() {
      return teebox;
    }
  }
}
