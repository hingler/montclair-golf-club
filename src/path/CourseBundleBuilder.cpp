#include "path/CourseBundleBuilder.hpp"
#include "corrugate/FeatureBox.hpp"
#include "path/deps/ControlPointCalculator.hpp"
#include "path/deps/impl/SimpleControlPointCalculator.hpp"

// prob. that two stop points will be disconnected
#define STOP_BREAK_PROBABILITY 0.2

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
    std::mt19937_64& engine,
    const cg::FeatureBox& box
  ) const {
    CourseBundle bundle;

    if (point_list.size() < 2) {
      // no changes.
      return bundle;
    }

    bundle.course_path = point_list;

    for (auto itr = ++bundle.course_path.begin(); itr < bundle.course_path.end();) {
      if (glm::length(*itr - *(itr - 1)) < 0.0001) {
        itr = bundle.course_path.erase(itr);
      } else {
        ++itr;
      }
    }
    bundle.stop_indices = control_calc->GetControlPoints(
      bundle.course_path,
      engine
    );

    // uni dist
    std::uniform_real_distribution<double> join_probability(0.0, 1.0);

    for (size_t i = 0; i < bundle.stop_indices.size(); i++) {
      size_t v = bundle.stop_indices.at(i);
      assert(v < bundle.course_path.size() && v >= 0);
      bundle.path_stops.push_back(bundle.course_path.at(v));
    }

    for (size_t i = 0; i < (bundle.stop_indices.size() - 1); i++) {
      if (join_probability(engine) < STOP_BREAK_PROBABILITY) {
        bundle.path_breaks.push_back(i);
      }
    }

    bundle.par = bundle.path_stops.size() + 1;

    glm::dvec2& ts = bundle.course_path.at(0);
    glm::dvec2& te = bundle.course_path.at(1);

    bundle.tee_direction = glm::normalize(te - ts);

    bundle.yardage = bundle.path_stops.size() + 1;

    bundle.origin = box.GetOrigin();

    std::uniform_real_distribution<double> hole_dist(-15.0, 15.0);

    bundle.hole = bundle.course_path.at(bundle.course_path.size() - 1) + glm::dvec2(
      hole_dist(engine),
      hole_dist(engine)
    );

    return bundle;
  }
}
