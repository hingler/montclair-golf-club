#ifndef SEED_PATH_H_
#define SEED_PATH_H_

#include "seed/path/node/PathNode.hpp"
#include "seed/path/impl/SeedPathIterator.hpp"

namespace mgc {
  // encompasses the path which a seed will eventually take
  class SeedPath {
   public:
    typedef SeedPathIterator iterator;
    typedef SeedPathIterator const_iterator;
    SeedPath(const glm::dvec2& init_point);

    // fetches the root of this path
    const PathNode* GetRoot() const;

    // tba: write iterator, and iterate via pts

    // iterator behavior
    SeedPathIterator begin() const;
    SeedPathIterator end() const;

    ~SeedPath();
   protected:
    PathNode* root;
  };
}

#endif // SEED_PATH_H_