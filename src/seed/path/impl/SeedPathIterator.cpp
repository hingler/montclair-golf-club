#include "seed/path/impl/SeedPathIterator.hpp"
#include "seed/path/node/BranchNode.hpp"

namespace terraingen {
  SeedPathIterator::SeedPathIterator() {
    path_cur = -1;
    current_path = nullptr;
  }

  SeedPathIterator::SeedPathIterator(PathNode* root_path) {
    path_cur = 0;
    current_path = root_path;
  }

  SeedPathIterator& SeedPathIterator::operator++() {
    ++path_cur;
    update_path();

    return *this;
  }

  SeedPathIterator SeedPathIterator::operator++(int) {
    SeedPathIterator copy(*this);

    ++path_cur;
    update_path();

    return copy;
  }

  SeedPathIterator::value_type SeedPathIterator::operator*() {
    if (current_path != nullptr) {
      return current_path->get(path_cur);
    }

    return glm::dvec2(0);
  }

  bool SeedPathIterator::operator==(const SeedPathIterator& other) const {
    // nullptr path should be equivalent
    return (path_cur == other.path_cur && current_path == other.current_path) || 
    (other.current_path == nullptr && current_path == nullptr);
  }

  bool SeedPathIterator::operator!=(const SeedPathIterator& other) const {
    return !(*this == other);
  }

  void SeedPathIterator::update_path() {
    if (current_path != nullptr) {
      if (path_cur >= current_path->size()) {
        BranchNode* branch = current_path->next;
        if (branch != nullptr) {
          branch_stack.push(branch->main);
          current_path = branch->branch;
        } else {
          // end of branch
          if (!branch_stack.empty()) {
            current_path = branch_stack.top();
            branch_stack.pop();
            path_cur = 0;
          } else {
            // end of path!
            path_cur = -1;
            current_path = nullptr;
          }
        }
      }
    }
  }
}