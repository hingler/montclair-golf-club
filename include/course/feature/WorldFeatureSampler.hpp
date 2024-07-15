#ifndef WORLD_FEATURE_SAMPLER_H_
#define WORLD_FEATURE_SAMPLER_H_

// root which handles chunking
// - specify chunk size, positioner, builder, opt fac in ctor
// - fetch features, given region
//   - (pad by min size, will auto'ly handle chunking)
//   - also: means we can group all worldfeatures into a single prepared sampler
// - given root node, handle opt fac
// note: for opt fac, we need to know the FINAL height
// (make priority implicit - ie feature trumps successive feature inserted)

#include "corrugate/MultiSampler.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureOverlapTester.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "seed/hole/overlap/OverlapTester.hpp"
#include <glm/glm.hpp>

#include <mutex>
#include <unordered_set>

namespace mgc {
  namespace mgc_course {

    namespace mgc_gen {
      class WorldFeatureManager;
    }

    class WorldFeatureSampler {
      typedef cg::MultiSampler<WorldFeature> sampler_type;
     public:
      typedef sampler_type::output_type output_type;
      WorldFeatureSampler(
        const std::shared_ptr<WorldFeaturePositioner>& positioner,
        const std::shared_ptr<WorldFeatureBuilder>& builder,
        const std::shared_ptr<WorldFeatureOverlapTester>& overlap_tester,
        const std::shared_ptr<UnderlyingSamplerHeight>& underlying,
        int chunk_size
      );

      output_type GetFeatures(
        const glm::dvec2& origin,
        const glm::dvec2& size
      ) const;

      void GetFeatures(
        const glm::dvec2& origin,
        const glm::dvec2& size,
        output_type& output
      ) const;

      std::shared_ptr<WorldFeaturePositioner> GetPositioner() const;

      double Sample(double x, double y, double underlying) const;
      glm::vec4 SampleSplat(double x, double y, size_t index) const;
      float SampleGrassFill(double x, double y) const;
      float SampleTreeFill(double x, double y) const;
      double SampleOcclusion(double x, double y) const;

     private:
      output_type GetFeatures(
        const glm::dvec2& point
      ) const;

      void CacheChunk(
        const glm::ivec2& chunk
      ) const;

      void CacheFeatures(
        const std::vector<glm::ivec2>& chunks
      ) const;

      void CacheFeatures(
        const glm::dvec2& point
      ) const;

      void CacheFeatures(
        const glm::dvec2& origin,
        const glm::dvec2& size
      ) const;

      std::vector<glm::ivec2> GetChunks(
        const glm::dvec2& origin,
        const glm::dvec2& size
      ) const;

      mutable cg::MultiSampler<WorldFeature> feature_sampler;
      mutable std::unordered_set<glm::ivec2> chunk_cache;

      std::shared_ptr<WorldFeaturePositioner> positioner;
      std::shared_ptr<WorldFeatureBuilder> builder;
      std::shared_ptr<WorldFeatureOverlapTester> overlap_tester;
      std::shared_ptr<UnderlyingSamplerHeight> underlying;

      mutable std::mutex cache_mutex;

      int chunk_size;
    };
  }
}



#endif // WORLD_FEATURE_SAMPLER_H_
