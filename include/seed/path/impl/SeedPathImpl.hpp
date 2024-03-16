#ifndef SEED_PATH_IMPL_H_
#define SEED_PATH_IMPL_H_

#include "seed/path/SeedPath.hpp"

namespace mgc {
  class SeedPathImpl : public SeedPath {
    public:
     using SeedPath::SeedPath;

     PathNode* GetRoot_write() { return root; }
  };
}

#endif // SEED_PATH_IMPL_H_