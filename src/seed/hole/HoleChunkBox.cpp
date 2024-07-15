#include "seed/hole/HoleChunkBox.hpp"

#include "seed/hole/impl/chunk_hash.hpp"

#include <corrugate/FeatureBox.hpp>
#include <limits>

namespace mgc {
  HoleChunkBox::HoleChunkBox(const std::vector<HoleBox>& holes, const glm::ivec2& chunk) : HoleChunkBox(HoleChunkBox::GetBoundingBox(holes), holes, chunk) {}

  HoleChunkBox::HoleChunkBox(const GC_AABB& bb, const std::vector<HoleBox>& holes, const glm::ivec2& chunk)
  : cg::FeatureBox(bb.origin, bb.size, 1.0f, 0.0f), chunk(chunk), priority_hash(hash::chunk_hash(chunk)) {
    for (auto& hole : holes) {
      // cctor lole
      // whatever?
      auto const_ptr = this->holes.InsertBox(std::make_unique<HoleBox>(hole));
      holes_ordered.push_back(const_ptr);
    }
  }


  bool HoleChunkBox::Test(const glm::dvec2& point) const {
    typename cg::MultiSampler<HoleBox>::output_type output;
    this->holes.FetchPoint(point, output);
    for (auto& hole : output) {
      if (hole->TestPadding(point)) {
        return true;
      }
    }

    return false;
  }

  GC_AABB HoleChunkBox::GetBoundingBox(const std::vector<HoleBox>& holes) {
    GC_AABB res;

    // do i care about itr order? not really (should, ideally, be random)

    // - need to be able to test against it

    // max value sampled
    glm::dvec2 max(std::numeric_limits<double>::lowest());
    res.origin = glm::dvec2(std::numeric_limits<double>::max());

    for (const auto& hole : holes) {
      // use res->origin to track min
      res.origin = glm::min(res.origin, hole.GetOrigin());
      // oh lol
      max = glm::max(max, hole.GetEnd());
    }

    res.size = max - res.origin;
    return res;
  }
}
