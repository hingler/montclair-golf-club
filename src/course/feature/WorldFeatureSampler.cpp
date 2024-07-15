#include "course/feature/WorldFeatureSampler.hpp"
#include "corrugate/sampler/SampleWriterGeneric.hpp"
#include "corrugate/sampler/SmoothingMultiBoxSampler.hpp"
#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureOverlapTester.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "course/feature/gen/WorldFeatureManager.hpp"
#include "gog43/Logger.hpp"
#include <glm/glm.hpp>

namespace mgc {
  namespace mgc_course {
    // pass one more var: reference to a CC of the feature manager
    // - will store underlying sample data
    // - cache updates will populate universally
    WorldFeatureSampler::WorldFeatureSampler(
      const std::shared_ptr<WorldFeaturePositioner>& positioner,
      const std::shared_ptr<WorldFeatureBuilder>& builder,
      const std::shared_ptr<WorldFeatureOverlapTester>& overlap_tester,
      const std::shared_ptr<UnderlyingSamplerHeight>& underlying,
      int chunk_size
    ) : positioner(positioner), builder(builder), overlap_tester(overlap_tester), underlying(underlying), chunk_size(chunk_size) {}

    WorldFeatureSampler::output_type WorldFeatureSampler::GetFeatures(
      const glm::dvec2& origin,
      const glm::dvec2& size
    ) const {
      output_type output;
      GetFeatures(origin, size, output);
      return output;
    }

    void WorldFeatureSampler::GetFeatures(
      const glm::dvec2& origin,
      const glm::dvec2& size,
      output_type& output
    ) const {
      CacheFeatures(origin, size);

      feature_sampler.FetchRange(
        origin, size, output
      );
    }

    std::shared_ptr<WorldFeaturePositioner> WorldFeatureSampler::GetPositioner() const {
      return positioner;
    }

    double WorldFeatureSampler::Sample(double x, double y, double underlying) const {
      cg::SmoothingMultiBoxSampler<WorldFeature> box(GetFeatures(glm::dvec2(x, y)));
      return box.SampleHeight(x, y, underlying);
    }

    glm::vec4 WorldFeatureSampler::SampleSplat(double x, double y, size_t index) const {
      cg::SmoothingMultiBoxSampler<WorldFeature> box(GetFeatures(glm::dvec2(x, y)));
      return box.SampleSplat(x, y, index);
    }

    float WorldFeatureSampler::SampleGrassFill(double x, double y) const {
      cg::SmoothingMultiBoxSampler<WorldFeature> box(GetFeatures(glm::dvec2(x, y)));
      return box.SampleGrassFill(x, y);
    }

    float WorldFeatureSampler::SampleTreeFill(double x, double y) const {
      cg::SmoothingMultiBoxSampler<WorldFeature> box(GetFeatures(glm::dvec2(x, y)));
      return box.SampleTreeFill(x, y);
    }

    // occlusion should be an SDF
    double WorldFeatureSampler::SampleOcclusion(double x, double y) const {
      cg::SmoothingMultiBoxSampler<WorldFeature> box(GetFeatures(glm::dvec2(x, y)));
      double acc = std::numeric_limits<float>::max();
      auto itr = box.cbegin();
      while (itr != box.cend()) {
        acc = std::min(acc, (*itr++)->SampleOcclusion(x, y));
      }

      return acc;
    }

    WorldFeatureSampler::output_type WorldFeatureSampler::GetFeatures(
      const glm::dvec2& point
    ) const {
      return this->GetFeatures(point - 0.5, glm::dvec2(1.0));
    }

    void WorldFeatureSampler::CacheChunk(
      const glm::ivec2& chunk
    ) const {
      {
        if (chunk_cache.find(chunk) != chunk_cache.end()) {
          // already cached!
          return;
        }
      }

      chunk_cache.insert(chunk);

      glm::dvec2 origin(chunk * chunk_size);
      glm::dvec2 size(chunk_size);

      std::vector<FeatureSpec> positions = positioner->GetPoints(
        origin, size
      );

      gog43::print("cached features in chunk: ", chunk.x, ", ", chunk.y);

      std::vector<std::unique_ptr<WorldFeature>> features = builder->BuildFeatures(
        underlying,
        positions
      );

      for (auto& box : features) {
        feature_sampler.InsertBox(std::move(box));
      }

    }

    void WorldFeatureSampler::CacheFeatures(const std::vector<glm::ivec2>& chunks) const {
      // contention here
      std::vector<glm::ivec2> not_cached;

      // separate into two parts, so that we lock max 2x
      // second lock should only be called in rare cases

      {
        std::lock_guard lock(cache_mutex);
        // acquire lock when reading chunk set
        for (const auto& chunk : chunks) {
          if (chunk_cache.find(chunk) == chunk_cache.end()) {
            not_cached.push_back(chunk);
          }
        }
      }

      if (not_cached.size() > 0) {
        std::lock_guard lock(cache_mutex);
        for (const auto& chunk : not_cached) {
          CacheChunk(chunk);
        }
      }
    }

    void WorldFeatureSampler::CacheFeatures(const glm::dvec2& point) const {
      CacheFeatures(point - 0.5, glm::dvec2(1.0));
    }

    void WorldFeatureSampler::CacheFeatures(const glm::dvec2& origin, const glm::dvec2& size) const {
      glm::dvec2 padded_origin = origin - builder->GetMaxFeatureSize();
      glm::dvec2 padded_size = size + 2.0 * builder->GetMaxFeatureSize();

      // pad out, st we ensure we're fetching all features which could feasibly be within the specified box
      std::vector<glm::ivec2> chunks = GetChunks(padded_origin, padded_size);
      CacheFeatures(chunks);
    }

    std::vector<glm::ivec2> WorldFeatureSampler::GetChunks(
      const glm::dvec2& origin, const glm::dvec2& size
    ) const {
      std::vector<glm::ivec2> res;
      glm::dvec2 end = origin + size;
      glm::ivec2 chunk_floor(glm::floor((origin - 0.01) / static_cast<double>(chunk_size)));
      glm::ivec2 chunk_ceil(glm::ceil((end + 0.01) / static_cast<double>(chunk_size)));

      for (int y = chunk_floor.y; y < chunk_ceil.y; y++) {
        for (int x = chunk_floor.x; x < chunk_ceil.x; x++) {
          res.push_back(glm::ivec2(x, y));
        }
      }

      return res;
    }
  }
}
