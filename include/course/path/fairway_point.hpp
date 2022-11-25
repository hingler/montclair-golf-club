#ifndef FAIRWAY_POINT_H_
#define FAIRWAY_POINT_H_

#include <glm/glm.hpp>

namespace course {
  namespace path {
    /**
     * @brief Linked list like struct for defining points along a fairway
     */
    struct fairway_point {
      glm::vec2 point;
      fairway_point* next;
    };

    // allocate fairway point
    fairway_point* fairway_point_alloc(int size);

    // free fairway point
    void fairway_point_free(fairway_point* point);
  }
}

#endif