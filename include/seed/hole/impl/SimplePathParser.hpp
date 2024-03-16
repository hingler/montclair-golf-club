#ifndef SIMPLE_PATH_ITERATOR_H_
#define SIMPLE_PATH_ITERATOR_H_

#include "seed/path/SeedPath.hpp"
#include "seed/path/node/PathNode.hpp"

namespace mgc {
  namespace pathparser {
    std::vector<const PathNode*> parse(const std::vector<std::shared_ptr<SeedPath>>& paths);

  }
}

#endif // SIMPLE_PATH_ITERATOR_H_
