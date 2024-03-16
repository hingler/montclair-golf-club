#ifndef HOLE_TERRAIN_H_
#define HOLE_TERRAIN_H_

#include <memory>

namespace mgc {
  template <typename HeightType, typename SplatType>
  class HoleTerrain {
   public:
    // idk if im a fan of this
    // it makes sense though!
    // thinking: we want to slap each part of the hole in
    // splat sampler can be dumb here

    // alt1: come up with some mapping which lets us parsse multiple samplers direct
    // - (ie: instead of having to pull from a splat manager)
    // alt2: splattype returns a single-component sampler, instead of a manager-like - we'll see
    virtual std::shared_ptr<HeightType> GetHeightSampler() = 0;
    virtual std::shared_ptr<SplatType> GetSplatSampler() = 0;
    // ignore smooth - i think we'll apply it to the whole shape, if at all
  };
}

#endif // HOLE_TERRAIN_H_
