#ifndef SEED_PATH_ITERATOR_VALUE_H_
#define SEED_PATH_ITERATOR_VALUE_H_

#include <glm/glm.hpp>

namespace mgc {
  struct SeedPathIteratorValue {
    // point value
    glm::dvec2 point;

    // true if last point is a jump - otherwise, false
    bool jump;
  };
}

#endif // SEED_PATH_ITERATOR_VALUE_H_