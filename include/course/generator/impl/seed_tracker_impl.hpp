#ifndef SEED_TRACKER_UINT64_H_
#define SEED_TRACKER_UINT64_H_

#include <cinttypes>

namespace course {
  namespace generator {
    namespace impl {
      struct seed_tracker_uint64 {
        uint64_t seed;

        void PushSeed(uint32_t depth, uint32_t input);
        uint32_t PopSeed(uint32_t depth);
        void CopySeed(const seed_tracker_uint64& other);
      };
    }
  }
}

#endif // SEED_TRACKER_UINT64_H_