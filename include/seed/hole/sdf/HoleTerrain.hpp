#ifndef HOLE_TERRAIN_H_
#define HOLE_TERRAIN_H_

#include <memory>

namespace mgc {
  template <typename HeightType, typename SplatType>
  class HoleTerrain {
   public:
    virtual std::shared_ptr<HeightType> GetHeightSampler() = 0;
    virtual std::shared_ptr<SplatType> GetSplatSampler() = 0;
    // ignore smooth - i think we'll apply it to the whole shape, if at all
  };
}

#endif // HOLE_TERRAIN_H_
