#include "seed/hole/impl/SeedChunkFactory.hpp"
#include "seed/hole/HoleBox.hpp"
#include "seed/hole/impl/SimplePathParser.hpp"
#include <random>

#include "gog43/Logger.hpp"

using namespace gog43;

namespace mgc {

  std::unique_ptr<HoleChunkBox> SeedChunkFactory::Create(const glm::ivec2& chunk) {

    glm::dvec2 chunk_origin = static_cast<glm::dvec2>(chunk) * config.chunk_size;
    glm::dvec2 chunk_ceil = chunk_origin + glm::dvec2(config.chunk_size);

    // figure out the first seed in this chunk

    // chunk origin wrt seed drops
    double seed_y = (chunk_origin.y - config.seed_origin.y) / config.seed_dist;

    glm::dvec2 first_seed = (chunk_origin - config.seed_origin) / config.seed_dist;
    glm::ivec2 first_seed_idx = static_cast<glm::ivec2>(glm::ceil(first_seed));

    // add 1.0 to cover offset
    glm::ivec2 last_seed_idx = first_seed_idx + glm::ivec2(glm::ceil(config.chunk_size / config.seed_dist) + 1.0);

    std::vector<SeedGrower::path_ptr> paths;

    auto seeds = GetSeeds(first_seed_idx, last_seed_idx, chunk_origin, chunk_ceil);
    for (auto& seed_point : seeds) {
      std::seed_seq sseq{ static_cast<int>(seed_point.x), static_cast<int>(seed_point.y), 32763 };
      auto paths_recurse = grower->GeneratePaths(seed_point, config_grow, sseq);

      // push generated paths onto return vec
      for (auto& path : paths_recurse) {
        paths.push_back(path);
      }
    }
    // lastly: generate hole bosxes from this
    std::vector<const PathNode*> nodes = pathparser::parse(paths);
    std::vector<HoleBox> results;



    // something about this is a bit "wordy" - might generate a list of seeds in a separate func and return that
    for (const PathNode*& node : nodes) {
      // bad values
      results.push_back(HoleBox(node, 48.0, 123));
    }

    return std::make_unique<HoleChunkBox>(results);
    // lastly: create boxes
  }

  std::vector<glm::dvec2> SeedChunkFactory::GetSeeds(
    const glm::ivec2& first_seed,
    const glm::ivec2& last_seed,
    const glm::dvec2& chunk_origin,
    const glm::dvec2& chunk_ceil
  ) const {
    std::vector<glm::dvec2> result;

    // upper bound seed count
    size_t seed_count = std::abs(last_seed.y - first_seed.y) * std::abs(last_seed.x - first_seed.x);

    result.reserve(seed_count);

    for (int sy = first_seed.y; sy < last_seed.y; sy++) {
      for (int sx = first_seed.x; sx < last_seed.x; sx++) {
        // check if seed is actually in the chunk
        // if so, grow a path for it
        // (in this case: wiggle would work just fine!)
        glm::dvec2 pos = GetSeedPosition(glm::ivec2(sx, sy));
        if (
          pos.x > chunk_origin.x
          && pos.y > chunk_origin.y
          && pos.x < chunk_ceil.x
          && pos.y < chunk_ceil.y
        ) {
          result.push_back(pos);
        }
      }
    }

    return result;
  }

  glm::dvec2 SeedChunkFactory::GetSeedPosition(const glm::ivec2& seed) const {
    return glm::dvec2(
      seed.x * config.seed_dist + (seed.x % 2) * (config.seed_dist / 2) + config.seed_origin.x,
      seed.y * config.seed_dist + config.seed_origin.y
    );
  }
}
