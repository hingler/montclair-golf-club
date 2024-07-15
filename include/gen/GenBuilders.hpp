#ifndef GEN_BUILDERS_H_
#define GEN_BUILDERS_H_

#include "course/feature/WorldFeatureBuilder.hpp"
#include "mgc/info/HoleInfoFetcher.hpp"
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

  struct WorldFeatureConfig {
    virtual void ConfigurePreGen(
      const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& pre_gen
    ) const = 0;

    // not great - but i'm fine w it tbh
    virtual void ConfigurePostGen(
      const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& post_gen,
      const std::shared_ptr<HoleInfoFetcher>& hole_info
    ) const = 0;
  };

  struct GenBuilder : public SeedGrowerBuilder, public ChunkConfigBuilder, public GrowConfigBuilder, public WorldFeatureConfig {};
}

#endif // GEN_BUILDERS_H_
