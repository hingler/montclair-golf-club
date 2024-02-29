#include "seed/hole/HoleChunkManager.hpp"

// i should probagly write tests for this hehe
// gonna write all the impl in one pass and then test it all at once
// (flow state hehe)
namespace mgc {
  HoleChunkManager::HoleChunkManager(
    const std::shared_ptr<SeedGrower>& grower,
    const ChunkConfig& config,
    const GrowConfig& config_grow
  ) : factory(grower, config, config_grow), config(config) {}

  void HoleChunkManager::FetchBoxes(
    const glm::dvec2& point,
    output_type& output
  ) {
    glm::ivec2 chunk = static_cast<glm::ivec2>(glm::floor(point / config.chunk_size));
    CacheNeighbors(chunk);
    hole_map.FetchPoint(point, output);
  }

  void HoleChunkManager::FetchBoxes(
    const glm::dvec2& origin,
    const glm::dvec2& size,
    output_type& output
  ) {
    // minimum chunk sampled
    glm::ivec2 chunk_floor = static_cast<glm::ivec2>(glm::floor(origin / config.chunk_size));
    // maximum chunk sampled
    glm::ivec2 chunk_ceil = static_cast<glm::ivec2>(glm::floor((origin + size) / config.chunk_size));
    for (int y = chunk_floor.y; y <= chunk_ceil.y; y++) {
      for (int x = chunk_floor.x; x <= chunk_ceil.x; x++) {
        // cache data for all possibly-intersecting holes
        CacheNeighbors(glm::ivec2(x, y));
      }
    }

    // fetch result based on range
    hole_map.FetchRange(origin, size, output);
  }

  HoleChunkManager::ptr_type HoleChunkManager::FetchNeighbors(
    const glm::ivec2& chunk,
    output_type& output
  ) {
    return CacheNeighbors(chunk, &output);
  }

  HoleChunkManager::ptr_type HoleChunkManager::CacheNeighbors(const glm::ivec2& chunk, output_type* output) {
    ptr_type c;
    for (int x = chunk.x - 1; x <= chunk.x + 1; x++) {
      for (int y = chunk.y - 1; y <= chunk.y + 1; y++) {
        glm::ivec2 chunk_coord(x, y);
        ptr_type res = GenerateOrFetch(chunk_coord);
        if (x == chunk.x && y == chunk.y) {
          c = res;
        } else if (output != nullptr) {
          output->insert(res);
        }
      }
    }

    return c;
  }

  HoleChunkManager::ptr_type HoleChunkManager::Insert(const glm::ivec2& chunk, std::unique_ptr<HoleChunkBox> box) {
    ptr_type insert_result = hole_map.InsertBox(std::move(box));
    hole_cache.insert(std::make_pair(chunk, insert_result));
    return insert_result;
  }

  HoleChunkManager::ptr_type HoleChunkManager::GenerateOrFetch(const glm::ivec2& chunk) {
    auto itr = hole_cache.find(chunk);
    if (itr != hole_cache.end()) {
      return itr->second;
    }

    std::unique_ptr<HoleChunkBox> res = factory.Create(chunk);
    return Insert(chunk, std::move(res));
  }
}
