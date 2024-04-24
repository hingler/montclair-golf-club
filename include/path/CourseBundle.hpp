#ifndef COURSE_BUNDLE_H_
#define COURSE_BUNDLE_H_

#include <vector>

#include <glm/glm.hpp>

namespace mgc {
  // bundle together impl defining the course
  struct CourseBundle {

    // path for the hole
    std::vector<glm::dvec2> course_path;

    // first stop is the tee
    std::vector<glm::dvec2> path_stops;

    // indices on course path corresponding with path stops
    std::vector<size_t> stop_indices;

    // direction which tee box will face
    glm::dvec2 tee_direction;

    // position of the hole - deviates from last point on course path
    glm::dvec2 hole;

    // marks the stops at which we want to "fragment" our in-bounds area
    std::vector<size_t> path_breaks;
    // (tba: shouldn't be out of bounds necc - for now it's fine though)
    // (down the line: how do we want to delineate oob/unsmoothed??)
    // - handle it based on underlying terrain (semi-ok - lots of IB territory)
    // - i kinda like that - just do what the ground below us does
    //   - but: should we ever mark "unmanaged" grass which is outside the course territory??
    //   - idea1: down the line, create a "course bounds" in another splat
    //     - let the generator determine what splat it ought to be, down the line...

    // yardage of the entire course
    double yardage;

    // par for this course
    size_t par;

    // origin point for this course bundle
    glm::dvec2 origin;

    double IndexAtDist(double yards) const;
    glm::dvec2 SampleAtIndex(double index) const;
    glm::dvec2 SampleAtDist(double yards) const;
    glm::dvec2 GetTangent(double index) const;
    glm::dvec2 GetNormal(double index) const;

    double IndexToYardage(size_t index) const;

    // fetches the index which is closest to some dist away from init index
    size_t GetNearestIndex(size_t init, double yardage) const;
  };
}

#endif // COURSE_BUNDLE_H_
