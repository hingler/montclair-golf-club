#ifndef HOLE_CHUNK_CONVERTER_H_
#define HOLE_CHUNK_CONVERTER_H_

#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "gog43/Logger.hpp"
#include "seed/ChunkConfig.hpp"
#include "seed/hole/HoleChunkBox.hpp"
#include "seed/hole/HoleChunkManager.hpp"
#include "seed/hole/SDFHoleBox.hpp"

#include "seed/hole/overlap/SDFGenOverlapTester.hpp"
#include "seed/hole/impl/SimpleHoleGenerator.hpp"

#include <memory>
namespace mgc {
  class HoleChunkConverter {
    // manages the conversion of hole chunks (from manager) to sample boxes
    // should we go point/range -> samplers? i'm thinking so
   public:
    typedef std::vector<std::unique_ptr<cg::BaseSmoothingSamplerBox>> output_type;
    HoleChunkConverter(
      const std::shared_ptr<HoleChunkManager>& manager,
      const ChunkConfig& config
    );

    template <typename HeightType>
    std::vector<std::unique_ptr<cg::BaseSmoothingSamplerBox>> Convert(
      const std::shared_ptr<const HoleChunkBox>& box,
      const std::shared_ptr<HeightType>& height
    ) const {
      // get chunk vec
      const glm::ivec2& chunk = box->chunk;
      HoleChunkManager::output_type output;
      // ignore return value - already
      {
        auto res = manager->FetchNeighbors(chunk, output);
        assert(res == box);
      }

      // create sdf multisampler
      auto sdfs = std::make_shared<cg::MultiSampler<SDFHoleBox>>();
      auto tester = std::make_shared<SDFGenOverlapTester>(sdfs, box, config);

      // add neighbors as hole chunks
      for (const auto& hole : output) {
        tester->AddHoleChunk(hole);
      }

      SimpleHoleGenerator<HeightType> gen(height, tester);

      // generate each individual hole
      for (const auto& hole_box : (*box)) {
        // are the boxes redundant at *this* point? i wouldn't think so, because they should be avoiding each other
        std::unique_ptr<SDFHoleBox> sdf = gen.GenerateHole(*hole_box);
        // sometimes null, if a hole shouldn't be generated
        if (sdf != nullptr) {
          // one insert - should be non-dupe
          sdfs->InsertBox(std::move(sdf));
        }
      }

      std::vector<std::unique_ptr<cg::BaseSmoothingSamplerBox>> result;
      for (const auto& sdf : (*sdfs)) {
        // convert finalized sdfs into sampler boxes
        // possibly: iterator is returning duplicates?? (no - ptrs are stored in a set, no dupes)
        result.push_back(sdf->Convert());
      }

      return result;
    }
   private:

    std::shared_ptr<HoleChunkManager> manager;
    const ChunkConfig config;
  };
}

#endif // HOLE_CHUNK_CONVERTER_H_
