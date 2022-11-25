#include "course/generator/impl/seed_tracker_impl.hpp"

namespace course {
  namespace generator {
    namespace impl {
      void seed_tracker_uint64::PushSeed(uint32_t depth, uint32_t input) {
        seed <<= depth;
        seed |= (input & (1 << (depth - 1)));
      }

      uint32_t seed_tracker_uint64::PopSeed(uint32_t depth) {
        uint32_t res = seed & (1 << (depth - 1));
        seed >>= depth;
        return res;
      }

      void seed_tracker_uint64::CopySeed(const seed_tracker_uint64& other) {
        
      }
    }
  }
}