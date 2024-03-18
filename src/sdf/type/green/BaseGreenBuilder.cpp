#include "sdf/type/green/BaseGreenBuilder.hpp"
#include "sdf/CPPBundle.hpp"

namespace mgc {
  namespace green {
    std::shared_ptr<CPPBundle> BaseGreenBuilder::Get(
      const CourseBundle& bundle,
      std::mt19937_64& engine
    ) const {
      // g
      // - scatter around tee location, and line it up roughly w course path? (cover tee loc and path)

      // ensure tee location is covered
      // ensure center is covered
      auto res = std::make_shared<CPPBundle>(25.0);
      // - generate in large range around end of path
      // - ensure generation reaches the hole
      // - give the center a few circles, and give the hole a couple smaller ones

      std::uniform_real_distribution<double> offset_end(-24.0, 24.0);
      std::normal_distribution<double> radius_end(14.0, 5.5);
      std::uniform_int_distribution<int> end_count(3, 5);

      int count = end_count(engine);

      GenerateGreenPatches(
        bundle.course_path.at(bundle.course_path.size() - 1),
        offset_end,
        radius_end,
        count,
        engine,
        res
      );

      std::uniform_real_distribution<double> offset_hole(-17.0, 17.0);
      std::normal_distribution<double> radius_hole(11.0, 3.5);

      GenerateGreenPatches(
        bundle.hole,
        offset_hole,
        radius_hole,
        4,
        engine,
        res
      );

      // (tba: check later hehe)
      return res;
    }

    void BaseGreenBuilder::GenerateGreenPatches(
      const glm::dvec2& origin,
      std::uniform_real_distribution<double>& offset_xy,
      std::normal_distribution<double>& radius,
      size_t patch_count,
      std::mt19937_64& engine,
      const std::shared_ptr<CPPBundle>& output
    ) const {
      for (int i = 0; i < patch_count; i++) {
        glm::dvec2 center = glm::dvec2(
          offset_xy(engine),
          offset_xy(engine)
        ) + origin;

        double rad = radius(engine);
        output->AddCircle(center.x, center.y, rad);
      }
    }
  }
}
