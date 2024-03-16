#ifndef PLACEHOLDER_TERRAIN_H_
#define PLACEHOLDER_TERRAIN_H_

#include "seed/hole/sdf/HoleTerrain.hpp"

namespace mgc {

  namespace _impl {
    struct ZeroSampler {
      double Sample(double x, double y) const {
        return 0.0;
      }
    };
  }
  class PlaceholderTerrain : public HoleTerrain<_impl::ZeroSampler, _impl::ZeroSampler> {
    std::shared_ptr<_impl::ZeroSampler> GetHeightSampler() { return std::make_shared<_impl::ZeroSampler>(); }
    std::shared_ptr<_impl::ZeroSampler> GetSplatSampler() { return std::make_shared<_impl::ZeroSampler>(); }
  };
}

#endif // PLACEHOLDER_TERRAIN_H_
