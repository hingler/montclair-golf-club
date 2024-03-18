#ifndef SDF_GEN_OVERLAP_TESTER_H_
#define SDF_GEN_OVERLAP_TESTER_H_

#include "seed/ChunkConfig.hpp"
#include "seed/hole/SDFHoleBox.hpp"
#include "seed/hole/overlap/OverlapTester.hpp"

#include "seed/hole/HoleChunkBox.hpp"

#include "corrugate/MultiSampler.hpp"

#include <memory>
#include <vector>

// provide as a builder? a bit iffy
namespace mgc {
  class SDFGenOverlapTester : public OverlapTester {
   public:
    SDFGenOverlapTester(
      const std::shared_ptr<cg::MultiSampler<SDFHoleBox>>& sdf,
      const std::shared_ptr<const HoleChunkBox>& center,
      const ChunkConfig& chunk_config
    );
    void AddHoleChunk(const std::shared_ptr<const HoleChunkBox>& box);
    bool Test(const glm::dvec2& point) const override;
   private:
    std::vector<std::shared_ptr<const HoleChunkBox>> neighbors;
    std::shared_ptr<cg::MultiSampler<SDFHoleBox>> sdfs;

    const std::shared_ptr<const HoleChunkBox> center;
    const ChunkConfig config;
    // how to store sdf holes that are being passed in?
    // - alt: pass a ptr to a multisampler which is storing them for us?
  };
}

#endif // SDF_GEN_OVERLAP_TESTER_H_
