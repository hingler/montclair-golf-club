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

    // direction which tee box will face
    glm::dvec2 tee_direction;

    // yardage of the entire course
    double yardage;

    // par for this course
    size_t par;
  };
}

#endif // COURSE_BUNDLE_H_
