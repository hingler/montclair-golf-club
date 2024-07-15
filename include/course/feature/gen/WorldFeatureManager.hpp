#ifndef WORLD_FEATURE_MANAGER_H_
#define WORLD_FEATURE_MANAGER_H_

#include "chunker/ChunkIdentifier.hpp"
#include "corrugate/sampler/SampleWriterGeneric.hpp"
#include "course/feature/SamplerBuilder.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureInstancer.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "course/feature/WorldFeatureSampler.hpp"
#include "course/feature/gen/FeatureInstanceSpawner.hpp"
#include "course/feature/gen/FeatureSubSampler.hpp"
#include <glm/glm.hpp>

#include <cstddef>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace mgc {
  namespace mgc_course {
    namespace mgc_gen {
      // stores list of features!
      class WorldFeatureManager {
       public:
        typedef size_t FeatureId;

        template <typename HeightType>
        WorldFeatureManager(
          const std::shared_ptr<HeightType>& height
        ) : base_height(
          std::make_shared<cg::SampleWriterGenericImpl<double, HeightType>>(height)
        ) {}
        double Sample(double x, double y, double underlying) const;
        glm::vec4 SampleSplat(double x, double y, size_t index) const;
        float SampleGrassFill(double x, double y) const;
        float SampleTreeFill(double x, double y) const;
        double SampleOcclusion(double x, double y) const;

        // add a feature to this manager
        void AddFeature(
          const std::shared_ptr<WorldFeatureBuilder>& builder,
          const std::shared_ptr<WorldFeaturePositioner>& positioner,
          FeatureId id,
          size_t chunk_size
        );

        void AddFeature(
          const SamplerBuilder& builder,
          FeatureId id,
          size_t chunk_size
        );

        std::shared_ptr<FeatureInstanceSpawner> Spawn(
          const std::shared_ptr<WorldFeatureInstancer>& instancer,
          FeatureId id
        );

        // tba: need to depend on chunker for chunk id hehe
        std::shared_ptr<FeatureSubSampler> Prepare(
          const glm::dvec2& origin,
          const glm::dvec2& size
        ) const;

        std::shared_ptr<FeatureSubSampler> Prepare(
          const chunker::ChunkIdentifier& id
        ) const;

       private:
        std::map<FeatureId, std::shared_ptr<WorldFeatureSampler>> samplers;
        std::shared_ptr<cg::SampleWriterGeneric<double>> base_height;
      };
    }
  }
}

#endif // WORLD_FEATURE_MANAGER_H_
