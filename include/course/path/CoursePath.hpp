#ifndef COURSE_PATH_H_
#define COURSE_PATH_H_

#include <glm/glm.hpp>

#include <vector>

namespace course {
  namespace path {
    struct CoursePath {
      std::vector<glm::vec2> course_path;

      /**
       * @brief Recenters this course path within the specified box
       * 
       * @param start - start of the bounding box
       * @param end - end of the bounding box
       */
      void Recenter(const glm::vec2& start, const glm::vec2& end);
    };
  }
}

#endif // COURSE_PATH_H_