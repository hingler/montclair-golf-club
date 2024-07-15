#ifndef BASIC_BUILDER_H_
#define BASIC_BUILDER_H_

#include "course/feature/WorldFeatureBuilder.hpp"
#include "gen/GenBuilders.hpp"
#include "seed/ChunkConfig.hpp"
#include "seed/GrowConfig.hpp"
#include "seed/direct/GradNormalDirector.hpp"
#include "course/feature/gen/WorldFeatureManager.hpp"
#include <memory>
namespace mgc {
  template <typename HeightType>
  class BasicBuilder : public GenBuilder {
   public:
    BasicBuilder(
      const std::shared_ptr<HeightType>& height
    ) : height(height) {

    }

    std::shared_ptr<SeedGrower> GetGrower() const override {
      auto res = std::make_shared<SeedGrower>();
      res->AddDirector<GradNormalDirector<HeightType>>(
        height,
        1.0
      );

      return res;
    }

    ChunkConfig GetChunkConfig() const override {
      ChunkConfig res;
      res.chunk_size = 4096.0;
      res.box_padding = 240.0;
      res.seed_dist = 2064.0;
      res.seed_origin = glm::dvec2(0.0);

      return res;
    }

    GrowConfig GetGrowConfig() const override {
      GrowConfig res;

      res.base_energy = 3125.0;
      res.branch_angle_degs = 56.0;
      res.branch_probability = 0.4;

      res.rigidity = 0.991;
      res.step_size = 9.6;

      return res;
    }

    void ConfigurePreGen(
      const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& pre_gen
    ) const override {}

    void ConfigurePostGen(
      const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& post_gen,
      const std::shared_ptr<HoleInfoFetcher>& hole_info
    ) const override {

    }

   private:
    std::shared_ptr<HeightType> height;
  };
}

#endif // BASIC_BUILDER_H_
