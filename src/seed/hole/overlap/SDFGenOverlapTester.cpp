#include "seed/hole/overlap/SDFGenOverlapTester.hpp"

#include "seed/hole/impl/chunk_hash.hpp"
#include "util/AABB.hpp"

namespace mgc {
  SDFGenOverlapTester::SDFGenOverlapTester(
    const std::shared_ptr<cg::MultiSampler<SDFHoleBox>>& sdfs,
    const std::shared_ptr<const HoleChunkBox>& center,
    const ChunkConfig& chunk_config
  ) : sdfs(sdfs), center(center), config(chunk_config) {}

  void SDFGenOverlapTester::AddHoleChunk(const std::shared_ptr<const HoleChunkBox>& box) {
    neighbors.push_back(box);
  }

  bool SDFGenOverlapTester::Test(const glm::dvec2& point) const {
    for (auto& neighbor : neighbors) {
      // whether center chunk should appear on top
      bool higher_priority = hash::chunk_override(*center, *neighbor);

      if (!higher_priority && neighbor->Test(point)) {
        return true;
      }
    }

    cg::MultiSampler<SDFHoleBox>::output_type intersect_test;

    sdfs->FetchPoint(point, intersect_test);
    for (auto& sdf : intersect_test) {
      if (sdf->Dist(point) < config.box_padding) {
        return true;
      }
    }
    return false;
  }
}
