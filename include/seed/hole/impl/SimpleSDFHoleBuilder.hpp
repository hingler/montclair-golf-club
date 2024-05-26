#ifndef SIMPLE_SDF_HOLE_BUILDER_H_
#define SIMPLE_SDF_HOLE_BUILDER_H_

#include "corrugate/sampler/smooth/LocalizedSmoother.hpp"
#include "path/CourseBundleBuilder.hpp"

#include "sdf/CPPBundle.hpp"
#include "sdf/type/SDFBuilder.hpp"
#include "sdf/type/fairway/BaseFairwayBuilder.hpp"

#include "sdf/type/green/BaseGreenBuilder.hpp"
#include "sdf/type/rough/BaseRoughBuilder.hpp"
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

      // defining contiguity??
      // - arr of (arr of points)
      // - define disjoints only (ie: `0` is a default, `1` implies a disjoint at 1 -> 2)

      // also: does rough need to know what fairway is doing?? (or can we just min the base fairway on top with some padding???)
      // - thinking this
      //
      // ergo:
      // - rough knows where our discontinuities are (from bundler)
      // - fairway does too
      // - ergo: rough knows how to bundle "un-like" regions
      // for smoothing: use wgt'd average (requires a visible smoothing fac)

      auto fairway_builder = fairway::BaseFairwayBuilder();
      auto sand_builder = sand::BaseSandBuilder();
      auto green_builder = green::BaseGreenBuilder();
      auto rough_builder = rough::BaseRoughBuilder();

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

      std::shared_ptr<rough::BaseRoughBuilder::sdf_type> rough = rough_builder.Get(
        bundle,
        engine
      );

      auto builder = SDFBuilder(
        fairway,
        green,
        sand,
        rough
      );


      std::shared_ptr<cg::smooth::LocalizedSmoother<HeightType, CPPBundle>> smoother =
        std::make_shared<cg::smooth::LocalizedSmoother<HeightType, CPPBundle>>(
          height, box.GetOrigin()
        );

      for (size_t i = 0; i < rough->Bundles(); i++) {
        std::shared_ptr<CPPBundle> bundle = rough->GetBundle(i);

        if (bundle != nullptr) {
          glm::dvec4 bb = bundle->GetBoundingBox();

          glm::dvec2 start(bb.x, bb.y);
          glm::dvec2 end(bb.z, bb.w);

          smoother->AddSubSmoother(
            bundle,
            start,
            end,
            45.0,
            0.042
          );
        }
      }

      glm::dvec4 green_bb = green->GetBoundingBox();
      glm::dvec2 green_start(green_bb.x, green_bb.y);
      glm::dvec2 green_end(green_bb.z, green_bb.w);

      // add one more smoother for green
      smoother->AddSubSmoother(
        green,
        green_start,
        green_end,
        32.0,
        0.021
      );

      // prob want a smooth patch for just the green


      // use rough to define "smoothing cells"
      // - each one will determine its own "flatten" point
      // - i think we should make the last one a bit flatter (for the green)
      return std::make_unique<
        SDFHoleBoxImpl<
          SDFBuilder::fairway_type,
          SDFBuilder::green_type,
          SDFBuilder::sand_type,
          SDFBuilder::rough_type,
          HeightType,
          CPPBundle
        >
      >(
        box,
        builder.GetFairway(),
        builder.GetGreen(),
        builder.GetSand(),
        builder.GetRough(),
        height,
        smoother,
        bundle
      );

      // (fix rough overlapping with sand/etc)
      // (alt: subtract sand and fairway in construction)
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
