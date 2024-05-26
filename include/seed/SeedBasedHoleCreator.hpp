#ifndef SEED_BASED_HOLE_CREATOR_H_
#define SEED_BASED_HOLE_CREATOR_H_

#include "corrugate/MultiSampler.hpp"
#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/sampler/DataSampler.hpp"
#include "corrugate/sampler/SmoothingMultiBoxSampler.hpp"
#include "glm/fwd.hpp"
#include "path/CourseBundle.hpp"
#include "seed/hole/ConversionResult.hpp"
#include "seed/hole/HoleChunkConverter.hpp"
#include "seed/hole/HoleChunkManager.hpp"
#include <memory>
#include <mutex>

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
   public:
    typedef cg::SmoothingMultiBoxSampler<cg::BaseSmoothingSamplerBox> sampler_type;
    SeedBasedHoleCreator(
      const std::shared_ptr<HeightMap>& height,
      const std::shared_ptr<SeedGrower>& grower,
      const ChunkConfig& config,
      const GrowConfig& config_grow
    ) : manager(std::make_shared<HoleChunkManager>(
      grower,
      config,
      config_grow
    )), converter(
      manager,
      config
    ), height(height) {
      // build seed grower here? nah
      // should be handled from outside this
    }

    // Sample height of underlying holes
    float SampleHeight(double x, double y, double underlying) const {
      sampler_type holes = GetHoleSampler(glm::dvec2(x, y));
      return holes.SampleHeight(x, y, underlying);
    }

    glm::vec4 SampleSplat(double x, double y, size_t index) const {
      sampler_type holes = GetHoleSampler(glm::dvec2(x, y));
      return holes.SampleSplat(x, y, index);
    }

    float SampleTreeFill(double x, double y) const {
      sampler_type holes = GetHoleSampler(glm::dvec2(x, y));
      return holes.SampleTreeFill(x, y);
    }

    float SampleGrassFill(double x, double y) const {
      return GetHoleSampler(glm::dvec2(x, y)).SampleTreeFill(x, y);
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
      sampler_type holes = GetHoleSampler(origin, size);
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
      sampler_type holes = GetHoleSampler(origin, size);
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
      sampler_type holes = GetHoleSampler(origin, size);
      return holes.WriteTreeFill(
        origin,
        sample_dims,
        scale,
        output,
        n_bytes
      );
    }

    sampler_type GetHoleSampler(
      const glm::dvec2& point
    ) const {
      HoleChunkManager::output_type chunks;
      manager->FetchBoxes(point, chunks);
      UpdateChunks(chunks);

      sampler_output output;
      holes.FetchPoint(point, output);
      return sampler_type(output);
    }

    sampler_type GetHoleSampler(
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

    std::vector<HoleInfo> GetHoleInfo(
      const glm::dvec2& origin,
      const glm::dvec2& size
    ) const {
      HoleChunkManager::output_type chunks;
      manager->FetchBoxes(origin, size, chunks);
      UpdateChunks(chunks);

      sampler_output output;
      holes.FetchRange(origin, size, output);

      std::vector<HoleInfo> result;

      {
        std::lock_guard<std::mutex> lock(bundle_mutex);
        for (const auto& ptr : output) {
          HoleInfo info;
          info.box = ptr;
          auto itr = course_bundles.find(ptr);

          // should always be true!
          assert(itr != course_bundles.end());
          info.data = itr->second;
          result.push_back(std::move(info));
        }
      }

      return result;
    }

   private:

    void UpdateChunks(const HoleChunkManager::output_type& chunks) const {
      for (auto& chunk : chunks) {
        // some case where this is called concurrently?? (ie: two threads trying to convert the same chunk)
        // no more warnings from logs - yippee!!!
        cache_type::LockHandle handle = box_cache.Acquire(chunk);
        if (!handle.Available()) {
          std::vector<ConversionResult> output = converter.Convert(chunk, height);

          {
            std::lock_guard<std::mutex> lock(bundle_mutex);
            // should be quick, and run infrequently - no actual work done
            for (ConversionResult& hole : output) {
              std::shared_ptr<const cg::BaseSmoothingSamplerBox> ptr = holes.InsertBox(std::move(hole.box));
              course_bundles.insert(std::make_pair(std::move(ptr), hole.data));
            }
          }

          box_cache.Release(handle, chunk->chunk);
        }
      }
    }

    // i really dislike the architecture on this - things feel like a mess
    // what would be the better way to do this???


    // hole chunk gens and fetches hole boxes
    // some other component dumb-converts

    std::shared_ptr<HeightMap> height;
    std::shared_ptr<HoleChunkManager> manager;

    mutable cg::MultiSampler<cg::BaseSmoothingSamplerBox> holes;
    typedef chunker::MutexCache<std::shared_ptr<const mgc::HoleChunkBox>, glm::ivec2> cache_type;
    mutable cache_type box_cache;

    // need a lock??
    mutable std::mutex bundle_mutex;
    mutable std::unordered_map<std::shared_ptr<const cg::BaseSmoothingSamplerBox>, CourseBundle> course_bundles;

    HoleChunkConverter converter;

  };
}

#endif // SEED_BASED_HOLE_CREATOR_H_
