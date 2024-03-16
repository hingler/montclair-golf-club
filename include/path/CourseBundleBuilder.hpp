#ifndef COURSE_BUNDLE_BUILDER_H_
#define COURSE_BUNDLE_BUILDER_H_

#include "corrugate/FeatureBox.hpp"
#include "path/CourseBundle.hpp"
#include "path/deps/ControlPointCalculator.hpp"

#include <memory>
#include <random>
#include <vector>

namespace mgc {
  // converts a series of points into a course bundle:
  // - moves contents of a path vector to struct
  // - determines yardage / par and creates stops for course building
  //
  class CourseBundleBuilder {
    // take path as is - if we need to fudge it, it should be someone else's responsibility
   public:
    // default ctor - use default dependencies
    CourseBundleBuilder();

    CourseBundleBuilder(
      std::unique_ptr<deps::ControlPointCalculator>&& calc
    );

    CourseBundle Convert(
      const std::vector<glm::dvec2>& point_list,
      std::mt19937_64& engine,
      const cg::FeatureBox& box
    ) const;

   private:
    std::unique_ptr<deps::ControlPointCalculator> control_calc;
  };
}

#endif // COURSE_BUNDLE_BUILDER_H_
