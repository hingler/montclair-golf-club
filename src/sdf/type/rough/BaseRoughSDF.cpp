#include "sdf/type/rough/BaseRoughSDF.hpp"

namespace mgc {
  namespace rough {
    BaseRoughSDF::BaseRoughSDF(
      const std::vector<std::shared_ptr<CPPBundle>>& bundles
    ) : bundles(std::move(bundles)) {}

    std::shared_ptr<CPPBundle> BaseRoughSDF::GetBundle(size_t index) const {
      if (index >= bundles.size()) {
        return nullptr;
      }

      return bundles[index];
    }

    double BaseRoughSDF::Sample(double x, double y) const {
      double min_dist = std::numeric_limits<double>::max();
      for (const auto& bundle : bundles) {
        // no smoothing hehe
        min_dist = std::min(min_dist, bundle->Sample(x, y));
      }

      return min_dist;
    }

    size_t BaseRoughSDF::Bundles() const {
      return bundles.size();
    }
  }
}
