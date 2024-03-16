#ifndef SEED_PATH_BUILDER_H_
#define SEED_PATH_BUILDER_H_

#include "seed/path/SeedPath.hpp"
#include "seed/path/impl/SeedPathImpl.hpp"

#include <memory>

namespace mgc {
  // initialize blank
  // initialize with a root path node to build off of
  // return a finalized path (as a seedpath ptr)
  class SeedPathBuilder {
   public:
    // empty seed path builder
    SeedPathBuilder(const glm::dvec2& init_point);
    std::shared_ptr<SeedPath> Build();

    void AddPoint(const glm::dvec2& point);

    // behavior invalid if path is terminated
    SeedPathBuilder AddBranch();
   private:
    // default ctor???
    SeedPathBuilder(const std::shared_ptr<SeedPathImpl>& root, PathNode* init_path);

    // if pathnode not provided: use underlying root
    PathNode* current_node;
    std::shared_ptr<SeedPathImpl> underlying;
  };
}

#endif // SEED_PATH_BUILDER_H_