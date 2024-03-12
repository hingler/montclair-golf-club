#include "path/CourseBundleBuilder.hpp"
#include "path/deps/ControlPointCalculator.hpp"
#include "path/deps/impl/SimpleControlPointCalculator.hpp"



namespace mgc {

  using namespace deps::impl;
  CourseBundleBuilder::CourseBundleBuilder() : CourseBundleBuilder(
    std::make_unique<SimpleControlPointCalculator>()
  ) {}

  CourseBundleBuilder::CourseBundleBuilder(
    std::unique_ptr<deps::ControlPointCalculator>&& calc
  ) : control_calc(std::move(calc)) {}
  CourseBundle CourseBundleBuilder::Convert(
    const std::vector<glm::dvec2>& point_list,
    size_t seed
  ) const {
    CourseBundle bundle;

    bundle.course_path = point_list;
    bundle.path_stops = control_calc->GetControlPoints(
      bundle.course_path,
      seed
    );

    bundle.par = bundle.path_stops.size() + 1;
  }
}
