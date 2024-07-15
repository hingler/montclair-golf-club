#include "sdf/type/rough/BaseRoughSDF.hpp"

namespace mgc {
  namespace rough {
    BaseRoughSDF::BaseRoughSDF(
      const std::vector<std::shared_ptr<CPPBundle>>& bundles
    ) : bundles(), indices() {
      for (int i = 0; i < bundles.size(); i++) {
        if (bundles.at(i) != nullptr) {
          indices.push_back(i);
          this->bundles.push_back(bundles.at(i));
        }
      }
    }

    std::shared_ptr<CPPBundle> BaseRoughSDF::GetBundle(size_t index) const {
      for (int i = 0; i < indices.size(); i++) {
        if (indices[i] == index) {
          return bundles[i];
        }
      }

      return nullptr;
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
