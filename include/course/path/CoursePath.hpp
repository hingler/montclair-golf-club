#ifndef COURSE_PATH_H_
#define COURSE_PATH_H_

#include <glm/glm.hpp>

#include <vector>

namespace course {
  namespace path {
    struct CoursePath {
      glm::vec2 tee;
      std::vector<glm::vec2> course_path;
    };
  }
}

#endif // COURSE_PATH_H_