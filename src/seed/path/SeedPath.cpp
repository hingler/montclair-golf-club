#include "seed/path/SeedPath.hpp"
#include "seed/path/node/BranchNode.hpp"

namespace terraingen {

  static void path_free(PathNode* node);
  static void branch_free(BranchNode* node);

  static void path_free(PathNode* node) {
    if (node != nullptr) {
      branch_free(node->next);

      delete node;
    }
  }

  static void branch_free(BranchNode* node) {
    if (node != nullptr) {
      path_free(node->branch);
      path_free(node->main);

      delete node;
    }
  }

  SeedPath::SeedPath(const glm::dvec2& init_point) {
    root = new PathNode(init_point);
  }

  const PathNode* SeedPath::GetRoot() const {
    return root;
  }

  SeedPathIterator SeedPath::begin() const {
    return SeedPathIterator(root);
  }

  SeedPathIterator SeedPath::end() const {
    return SeedPathIterator();
  }

  SeedPath::~SeedPath() {
    path_free(root);
  }
}