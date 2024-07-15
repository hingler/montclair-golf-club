#ifndef SIMPLE_SDF_HOLE_BUILDER_H_
#define SIMPLE_SDF_HOLE_BUILDER_H_

#include "corrugate/sampler/smooth/LocalizedSmoother.hpp"
#include "path/CourseBundle.hpp"
#include "path/CourseBundleBuilder.hpp"

#include "sdf/CPPBundle.hpp"
#include "sdf/type/SDFBuilder.hpp"
#include "sdf/type/fairway/BaseFairwayBuilder.hpp"

#include "sdf/type/green/BaseGreenBuilder.hpp"
#include "sdf/type/rough/BaseRoughBuilder.hpp"
#include "sdf/type/sand/BaseSandBuilder.hpp"

#include "seed/hole/SDFHoleBox.hpp"
#include "seed/hole/HoleBox.hpp"

#include <atomic>
#include <memory>
#include <random>

#include <gog43/Logger.hpp>

namespace mgc {
  template <typename HeightType>
  class SimpleSDFHoleBuilder {
   public:
    SimpleSDFHoleBuilder(std::shared_ptr<HeightType> height) : height(height), bundler() {}
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
      CourseBundle bundle = bundler.Convert(points, engine, box);


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

      // idea: do an initial "smoothing pass" on the course path??
      auto path_bundle = std::make_shared<CPPBundle>(24.0);
      path_bundle->AddCapsule(bundle.course_path, 48.0);

      glm::dvec4 bb = path_bundle->GetBoundingBox();

      // think of this as an "initial" pass to ensure the hole remains navigable
      // (will smooth more on top!)
      smoother->AddSubSmoother(
        path_bundle,
        glm::dvec2(bb.x, bb.y),
        glm::dvec2(bb.z, bb.w),
        35.0,
        0.241
      );

      // don't smooth final stop
      for (size_t i = 0; i < bundle.path_stops.size(); i++) {
        std::shared_ptr<CPPBundle> bundle = rough->GetBundle(i);

        if (bundle != nullptr) {
          glm::dvec4 bb = bundle->GetBoundingBox();

          glm::dvec2 start(bb.x, bb.y);
          glm::dvec2 end(bb.z, bb.w);

          // if green is smoothed by a smoother: dial back max slope?
          // break before green



          // randomize this a bit more
          // - tend lower
          smoother->AddSubSmoother(
            bundle,
            start,
            end,
            56.0,
            GetSmoothnessFactorFairway()
          );
        }
      }

      glm::dvec4 green_bb = green->GetBoundingBox();
      glm::dvec2 green_start(green_bb.x, green_bb.y);
      glm::dvec2 green_end(green_bb.z, green_bb.w);

      // add one more smoother for green (remove for now - i think its bugging)
      smoother->AddSubSmoother(
        green,
        green_start,
        green_end,
        54.0,
        GetSmoothnessFactorGreen()
      );

      // smooth tee
      std::shared_ptr<CPPBundle> tee_smooth = std::make_shared<CPPBundle>();
      const glm::dvec2& tee_origin = bundle.course_path.at(0);
      tee_smooth->AddCircle(tee_origin.x, tee_origin.y, 16.5);
      glm::dvec4 bounds = tee_smooth->GetBoundingBox();

      glm::dvec2 tee_start(bounds.x, bounds.y);
      glm::dvec2 tee_end(bounds.z, bounds.w);

      smoother->AddSubSmoother(
        tee_smooth,
        tee_start,
        tee_end,
        24.0,
        0.015
      );


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
    double GetSmoothnessFactorFairway() {
      static std::uniform_real_distribution<double> SMOOTH_FAC(0.023, 0.065);
      return SMOOTH_FAC(engine);
    }

    double GetSmoothnessFactorGreen() {
      static std::uniform_real_distribution<double> SMOOTH_FAC_GREEN(0.008, 0.019);
      return SMOOTH_FAC_GREEN(engine);
    }

    std::mt19937_64 engine;
    const std::shared_ptr<HeightType> height;

    // arb numbers
    CourseBundleBuilder bundler;
    static std::uniform_real_distribution<double> FAIRWAY_START;
    static constexpr double PAR3_THRESHOLD = 224.5;
    static constexpr double PAR4_THRESHOLD = 436.1;
    // tba: extend further
  };

  template <typename HeightType>
  std::uniform_real_distribution<double> SimpleSDFHoleBuilder<HeightType>::FAIRWAY_START = std::uniform_real_distribution<double>(95.0, 155.0);
}

#endif // SIMPLE_SDF_HOLE_BUILDER_H_
