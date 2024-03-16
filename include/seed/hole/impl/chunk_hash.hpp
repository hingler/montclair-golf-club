#ifndef CHUNK_HASH_H_
#define CHUNK_HASH_H_

#include "seed/hole/HoleChunkBox.hpp"
#include <glm/glm.hpp>

namespace mgc {
  namespace hash {
    // given a chunk ivec2, generates a hash
    size_t chunk_hash(const glm::ivec2& chunk);

    // returns true if `self` should appear on top of `other`
    bool chunk_override(const HoleChunkBox& self, const HoleChunkBox& other);
  }
}

#endif // CHUNK_HASH_H_
