#ifndef COURSE_PATH_TO_SPLINE_H_
#define COURSE_PATH_TO_SPLINE_H_

#include "course/path/CoursePath.hpp"
#include "course/path/CompoundCurve.hpp"

namespace course {
  namespace path {
    /**
     * @brief Creates a compound curve from a path.
     * 
     * @param path - path in question
     * @param tension - 0.5 is catmull rom, 0.0 is straight lines
     * @return CompoundCurve!
     */
    CompoundCurve CoursePathToCurve(const CoursePath& path, float tension);
  }
}

#endif // COURSE_PATH_TO_SPLINE_H_