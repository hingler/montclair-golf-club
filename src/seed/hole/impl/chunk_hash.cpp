#include "seed/hole/impl/chunk_hash.hpp"
#include "seed/hole/HoleChunkBox.hpp"

namespace mgc {
  namespace hash {
    size_t chunk_hash(const glm::ivec2& chunk) {
      glm::dvec2 chunk_d = static_cast<glm::dvec2>(chunk);

      double rand_res = glm::fract(glm::sin(glm::dot(chunk_d, glm::dvec2(12.9898, 78.233))) * 43758.5453);
      return static_cast<size_t>(glm::round(glm::abs(rand_res * 131072.0)));
    }

    // true if self should override
    bool chunk_override(const HoleChunkBox& self, const HoleChunkBox& other) {
      if (self.priority_hash == other.priority_hash) {
        if (self.chunk.x == other.chunk.x) {
          if (self.chunk.y == other.chunk.y) {
            // fallthru case - should never occur
            return false;
          } else {
            return (self.chunk.y < other.chunk.y);
          }
        } else {
          return (self.chunk.x < other.chunk.x);
        }
      } else {
        return (self.priority_hash > other.priority_hash);
      }
    }
  }
}
