#ifndef HOLE_CHUNK_MANAGER_H_
#define HOLE_CHUNK_MANAGER_H_

#include <corrugate/MultiSampler.hpp>


#include "seed/SeedGrower.hpp"
#include "seed/hole/impl/SeedChunkFactory.hpp"

#include "seed/hole/HoleChunkBox.hpp"

#include <unordered_map>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace mgc {
  // wraps hole chunk generation
  class HoleChunkManager {
   public:
    typedef std::shared_ptr<const HoleChunkBox> ptr_type;
    typedef std::unordered_set<ptr_type> output_type;
    HoleChunkManager(
      const std::shared_ptr<SeedGrower>& grower,
      const ChunkConfig& config,
      const GrowConfig& config_grow
    );

    // fetches all chunks which contain the specified point
    void FetchBoxes(
      const glm::dvec2& point,
      output_type& output
    );

    // fetches all chunks which overlap with the specified region
    void FetchBoxes(
      const glm::dvec2& origin,
      const glm::dvec2& size,
      output_type& output
    );

    // fetches all neighbors of a specified chunk
    ptr_type FetchNeighbors(
      const glm::ivec2& chunk,
      output_type& output
    );

    size_t chunk_count() const {
      return hole_map.size();
    }

    // need to

   private:
    ptr_type CacheNeighbors(const glm::ivec2& chunk, output_type* output = nullptr);

    ptr_type Insert(const glm::ivec2& chunk, std::unique_ptr<HoleChunkBox> box);

    ptr_type GenerateOrFetch(const glm::ivec2& chunk);

    SeedChunkFactory factory;
    ChunkConfig config;
    cg::MultiSampler<HoleChunkBox> hole_map;
    std::unordered_map<glm::ivec2, ptr_type> hole_cache;
  };
}

#endif // HOLE_CHUNK_MANAGER_H_
