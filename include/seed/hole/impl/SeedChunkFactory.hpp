#ifndef SEED_CHUNK_FACTORY_H_
#define SEED_CHUNK_FACTORY_H_

#include "seed/ChunkConfig.hpp"
#include "seed/hole/HoleChunkBox.hpp"
#include "seed/GrowConfig.hpp"
#include "seed/SeedGrower.hpp"

#include <memory>

namespace mgc {
  class SeedChunkFactory {
   public:
    SeedChunkFactory(const std::shared_ptr<const SeedGrower>& grower, const ChunkConfig& config, const GrowConfig& config_grow) : grower(grower), config(config), config_grow(config_grow) {}

    // also want to nudge up/down right
    std::unique_ptr<HoleChunkBox> Create(const glm::ivec2& chunk);
   private:
    glm::dvec2 GetSeedPosition(const glm::ivec2& seed) const;
    std::vector<glm::dvec2> GetSeeds(
      const glm::ivec2& first_seed,
      const glm::ivec2& last_seed,
      const glm::dvec2& chunk_origin,
      const glm::dvec2& chunk_ceil
    ) const;
    std::shared_ptr<const SeedGrower> grower;
    ChunkConfig config;
    GrowConfig config_grow;
  };

  // need code to call this
  // need code to convert hole chunk boxes to sdfs, one by one
  // need code to


  // focus only on smooth gen for now - use some stock terrain for now
}

#endif // SEED_CHUNK_FACTORY_H_
