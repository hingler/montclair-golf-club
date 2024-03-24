#ifndef COURSE_GEN_MULTI_H_
#define COURSE_GEN_MULTI_H_

// heightmap is the only thing we need to smooth off of
// (Let someone else manage it for now - we'll wrap later??)
#include "gen/BasicBuilder.hpp"
#include "gen/GenBuilders.hpp"
#include "mgc/impl/SeedHeightWrap.hpp"
#include "mgc/impl/SeedSplatWrap.hpp"
#include "seed/ChunkConfig.hpp"
#include "terrain/CourseStitcher.hpp"
namespace mgc {
  template <typename HeightType>
  class CourseGen_Multi {
   public:
    typedef const impl::SeedHeightWrap<HeightType> HeightSampler;
    typedef const impl::SeedSplatWrap<HeightType> SplatSampler;

    // what do we want to pass in???
    // - configure grower? i feel like we should be configuring it here
    CourseGen_Multi(
      const std::shared_ptr<HeightType>& height
    ) : CourseGen_Multi(height, BasicBuilder<HeightType>(height)) {}

    CourseGen_Multi(
      const std::shared_ptr<HeightType>& height,
      const GenBuilder& gen
    ) : CourseGen_Multi(
      height, gen, gen, gen
    ) {}

    CourseGen_Multi(
      const std::shared_ptr<HeightType>& height,
      const SeedGrowerBuilder& seed_builder,
      const ChunkConfigBuilder& chunk_builder,
      const GrowConfigBuilder& grow_builder
    ) : CourseGen_Multi(height, seed_builder.GetGrower(), chunk_builder.GetChunkConfig(), grow_builder.GetGrowConfig()) {}

    CourseGen_Multi(
      const std::shared_ptr<HeightType>& height,
      const std::shared_ptr<SeedGrower>& grower,
      const ChunkConfig& config,
      const GrowConfig& config_grow
    ) : stitcher(
      std::make_shared<CourseStitcher<HeightType>>(
        height, grower, config, config_grow
      )
    ),
    splat(std::make_shared<impl::SeedSplatWrap<HeightType>>(stitcher)),
    height(std::make_shared<impl::SeedHeightWrap<HeightType>>(stitcher))
    {}

    std::shared_ptr<HeightSampler> GetHeightMap() {
      return height;
    }

    std::shared_ptr<SplatSampler> GetSplatMap() {
      return splat;
    }

   private:
    std::shared_ptr<CourseStitcher<HeightType>> stitcher;

    std::shared_ptr<impl::SeedSplatWrap<HeightType>> splat;
    std::shared_ptr<impl::SeedHeightWrap<HeightType>> height;
  };
}

#endif // COURSE_GEN_MULTI_H_
