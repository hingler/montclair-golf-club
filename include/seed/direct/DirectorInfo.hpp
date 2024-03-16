#ifndef DIRECTOR_INFO_H_
#define DIRECTOR_INFO_H_

#include <glm/glm.hpp>

namespace mgc {
  struct DirectorInfo {
    // providing this to points so that they have ctx
    // at the same time: it's a bit unwieldy
    
    // position of present point
    glm::dvec2 point;

    // position of the root of this present branch
    glm::dvec2 root;

    // direction of the last step along this path
    glm::dvec2 direction;

    // (add as we go :3)
  };
}

// impls to write
// - gradient tan - pick gradient tangent in dir
// - wiggler - insert some tiny turbulent noise
// - ascender/descender - i think kinda like the wiggler, but spec. focusing on bumping the gradient up/down hills (?)

#endif // DIRECTOR_INFO_H_