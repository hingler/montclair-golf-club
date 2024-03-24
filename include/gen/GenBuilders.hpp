#ifndef GEN_BUILDERS_H_
#define GEN_BUILDERS_H_

#include "seed/ChunkConfig.hpp"
#include "seed/GrowConfig.hpp"
#include "seed/SeedGrower.hpp"

namespace mgc {
  struct SeedGrowerBuilder {
    virtual std::shared_ptr<SeedGrower> GetGrower() const = 0;
  };

  struct ChunkConfigBuilder {
    virtual ChunkConfig GetChunkConfig() const = 0;
  };

  struct GrowConfigBuilder {
    virtual GrowConfig GetGrowConfig() const = 0;
  };

  struct GenBuilder : public SeedGrowerBuilder, public ChunkConfigBuilder, public GrowConfigBuilder {};
}

#endif // GEN_BUILDERS_H_
