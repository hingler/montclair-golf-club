#ifndef HOLE_CHUNK_BOX_H_
#define HOLE_CHUNK_BOX_H_

#include <corrugate/FeatureBox.hpp>
#include <corrugate/MultiSampler.hpp>

#include "seed/hole/HoleBox.hpp"

namespace mgc {
  // Wraps some number of generated holes
  namespace _impl {
    // bounding box for hole data
    struct HoleBoxAABB {
      glm::dvec2 origin;
      glm::dvec2 size;
    };
  }

  class HoleChunkBox : public cg::FeatureBox {
   public:
    typedef cg::MultiSampler<HoleBox>::iterator   iterator;
    HoleChunkBox(const std::vector<HoleBox>& holes);

    iterator begin() const { return holes.begin(); }
    iterator end()   const { return holes.end();   }

    size_t chunk_count() const { return holes.size(); }

    // receive point in global space - return true if within padding range of holes
    // fiugh
    bool Test(const glm::dvec2& point);
   private:
    // priv call - receives pre-calculated origin and size
    HoleChunkBox(const _impl::HoleBoxAABB& bb, const std::vector<HoleBox>& holes);
    // contains holes, in global space
    static _impl::HoleBoxAABB GetBoundingBox(const std::vector<HoleBox>& holes);
    cg::MultiSampler<HoleBox> holes;
  };
}

#endif // HOLE_CHUNK_BOX_H_
