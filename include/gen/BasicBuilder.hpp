#ifndef BASIC_BUILDER_H_
#define BASIC_BUILDER_H_

#include "gen/GenBuilders.hpp"
#include "seed/ChunkConfig.hpp"
#include "seed/GrowConfig.hpp"
#include "seed/direct/GradNormalDirector.hpp"
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
      res.seed_dist = 1536.0;
      res.seed_origin = glm::dvec2(0.0);

      return res;
    }

    GrowConfig GetGrowConfig() const override {
      GrowConfig res;

      res.base_energy = 2080.0;
      res.branch_angle_degs = 60.0;
      res.branch_probability = 0.4;

      // tba: make distance constant
      res.rigidity = 0.98;
      res.step_size = 5.6;

      return res;
    }

   private:
    std::shared_ptr<HeightType> height;
  };
}

#endif // BASIC_BUILDER_H_
