#ifndef SIMPLE_SDF_HOLE_BUILDER_H_
#define SIMPLE_SDF_HOLE_BUILDER_H_

#include "path/CourseBundleBuilder.hpp"

#include "sdf/type/SDFBuilder.hpp"
#include "sdf/type/fairway/BaseFairwayBuilder.hpp"

#include "sdf/type/green/BaseGreenBuilder.hpp"
#include "sdf/type/sand/BaseSandBuilder.hpp"

#include "seed/hole/SDFHoleBox.hpp"
#include "seed/hole/HoleBox.hpp"

#include <memory>
#include <random>

#include <gog43/Logger.hpp>

namespace mgc {
  template <typename HeightType>
  class SimpleSDFHoleBuilder {
   public:
    SimpleSDFHoleBuilder(std::shared_ptr<HeightType> height) : height(height) {}
    std::unique_ptr<SDFHoleBox> CreateSDF(
      const HoleBox& box,
      const std::vector<glm::dvec2>& points,
      double len
    ) {
      // tba: now that we have our lib in place, we can jump straight to making proper boxes and converting them!
      // single thread constraint?
      engine.seed(box.seed);
      glm::dvec2 tee_direction = glm::normalize(points[1] - points[0]);

      // ignore for now
      double fairway_start = FAIRWAY_START(engine);

      auto bundler = CourseBundleBuilder();

      auto bundle = bundler.Convert(points, engine, box);

      auto fairway_builder = fairway::BaseFairwayBuilder();
      auto sand_builder = sand::BaseSandBuilder();
      auto green_builder = green::BaseGreenBuilder();

      auto fairway = fairway_builder.Get(
        bundle,
        engine
      );

      auto sand = sand_builder.Get(
        bundle,
        engine
      );

      auto green = green_builder.Get(
        bundle,
        engine
      );

      auto builder = SDFBuilder(
        fairway,
        green,
        sand
      );

      // distinct: separate dist sampler from threshold sampler
      return std::make_unique<
        SDFHoleBoxImpl<
          SDFBuilder::fairway_type,
          SDFBuilder::green_type,
          SDFBuilder::sand_type,
          HeightType
        >
      >(
        box,
        builder.GetFairway(),
        builder.GetGreen(),
        builder.GetSand(),
        height
      );
    }
   private:
    std::mt19937_64 engine;
    const std::shared_ptr<HeightType> height;

    // arb numbers
    static std::uniform_real_distribution<double> FAIRWAY_START;
    static constexpr double PAR3_THRESHOLD = 224.5;
    static constexpr double PAR4_THRESHOLD = 436.1;
    // tba: extend further
  };

  template <typename HeightType>
  std::uniform_real_distribution<double> SimpleSDFHoleBuilder<HeightType>::FAIRWAY_START = std::uniform_real_distribution<double>(95.0, 155.0);
}

#endif // SIMPLE_SDF_HOLE_BUILDER_H_
