#ifndef SEED_BASED_HOLE_CREATOR_H_
#define SEED_BASED_HOLE_CREATOR_H_

#include "corrugate/MultiSampler.hpp"
#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/sampler/DataSampler.hpp"
#include "corrugate/sampler/SmoothingMultiBoxSampler.hpp"
#include "seed/hole/HoleChunkConverter.hpp"
#include "seed/hole/HoleChunkManager.hpp"
#include <memory>

// this should be self-sufficient, and create course splats for us
// - give it some arb data and see what it does

// working!
// - gaps on overlap
//   - possible fix: arb priority rule (ie: only collide w/ higher-prio paths)

// bolstering generation (now that it works)
// - build paths more carefully - lay out shot locs
// - add green, sand traps

// tba1: want to add a prioritization system to chunks

namespace mgc {
  template <typename HeightMap>
  class SeedBasedHoleCreator {
    typedef cg::MultiSampler<cg::BaseSmoothingSamplerBox>::output_type sampler_output;
    typedef cg::SmoothingMultiBoxSampler<cg::BaseSmoothingSamplerBox> sampler_type;
   public:
    SeedBasedHoleCreator(
      std::shared_ptr<HeightMap> height,
      std::shared_ptr<SeedGrower>& grower,
      const ChunkConfig& config,
      const GrowConfig& config_grow
    ) : manager(std::make_shared<HoleChunkManager>(
      grower,
      config,
      config_grow
    )), converter(
      manager,
      config
    ) {
      // build seed grower here? nah
      // should be handled from outside this
    }

    // Sample height of underlying holes
    float SampleHeight(double x, double y, double underlying) {
      sampler_type holes = GetHoles(glm::dvec2(x, y));
      return holes.SampleHeight(x, y, underlying);
    }

    glm::vec4 SampleSplat(double x, double y, size_t index) const {
      sampler_type holes = GetHoles(glm::dvec2(x, y));
      return holes.SampleSplat(x, y, index);
    }

    float SampleTreeFill(double x, double y) const {
      sampler_type holes = GetHoles(glm::dvec2(x, y));
      return holes.SampleTreeFill(x, y);
    }

    size_t WriteHeight(
      const glm::dvec2& origin,
      const glm::ivec2& sample_dims,
      double scale,
      const cg::DataSampler<float>& underlying,
      float* output,
      size_t n_bytes
    ) const {
      glm::dvec2 size = glm::dvec2(sample_dims.x, sample_dims.y) * scale;
      sampler_type holes = GetHoles(origin, size);
      return holes.WriteHeight(
        origin,
        sample_dims,
        scale,
        underlying,
        output,
        n_bytes
      );
    }

    size_t WriteSplat(
      const glm::dvec2& origin,
      const glm::ivec2& sample_dims,
      double scale,
      size_t index,
      glm::vec4* output,
      size_t n_bytes
    ) const {
      glm::dvec2 size = glm::dvec2(sample_dims.x, sample_dims.y) * scale;
      sampler_type holes = GetHoles(origin, size);
      return holes.WriteSplat(
        origin,
        sample_dims,
        scale,
        index,
        output,
        n_bytes
      );
    }

    size_t WriteTreeFill(
      const glm::dvec2& origin,
      const glm::ivec2& sample_dims,
      double scale,
      float* output,
      size_t n_bytes
    ) const {
      glm::dvec2 size = glm::dvec2(sample_dims.x, sample_dims.y) * scale;
      sampler_type holes = GetHoles(origin, size);
      return holes.WriteTreeFill(
        origin,
        sample_dims,
        scale,
        output,
        n_bytes
      );
    }

   private:
    sampler_type GetHoles(
      const glm::dvec2& point
    ) const {
      HoleChunkManager::output_type chunks;
      manager->FetchBoxes(point, chunks);
      UpdateChunks(chunks);

      sampler_output output;
      holes.FetchPoint(point, output);
      return sampler_type(output);
    }

    sampler_type GetHoles(
      const glm::dvec2& origin,
      const glm::dvec2& size
    ) const {
      HoleChunkManager::output_type chunks;
      manager->FetchBoxes(origin, size, chunks);
      UpdateChunks(chunks);

      sampler_output output;
      holes.FetchRange(origin, size, output);
      return sampler_type(output);
    }

    void UpdateChunks(const HoleChunkManager::output_type& chunks) const {
      for (auto& chunk : chunks) {
        if (gen_cache.find(chunk) == gen_cache.end()) {
          std::vector<std::unique_ptr<cg::BaseSmoothingSamplerBox>> output = converter.Convert(chunk, height);
          for (std::unique_ptr<cg::BaseSmoothingSamplerBox>& hole : output) {
            holes.InsertBox(std::move(hole));
          }
          gen_cache.insert(chunk);
        }
      }
    }

    // i really dislike the architecture on this - things feel like a mess
    // what would be the better way to do this???


    // hole chunk gens and fetches hole boxes
    // some other component dumb-converts

    std::shared_ptr<HeightMap> height;
    std::shared_ptr<HoleChunkManager> manager;

    // mark as mutable - caches which should not affect external state
    mutable std::unordered_set<std::shared_ptr<const HoleChunkBox>> gen_cache;
    mutable cg::MultiSampler<cg::BaseSmoothingSamplerBox> holes;

    HoleChunkConverter converter;

  };
}

#endif // SEED_BASED_HOLE_CREATOR_H_
