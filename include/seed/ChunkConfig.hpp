#ifndef SEED_CONFIG_H_
#define SEED_CONFIG_H_

#include <glm/glm.hpp>

namespace mgc {
  struct ChunkConfig {
    // position of origin seed
    glm::dvec2 seed_origin;

    // distance between rows of seeds - approx. hex pattern
    double seed_dist;

    // size of each chunk, in units - origin is always 0
    double chunk_size;
  };
}

#endif // SEED_CONFIG_H_
