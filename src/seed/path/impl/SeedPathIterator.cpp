#include "seed/path/impl/SeedPathIterator.hpp"
#include "seed/path/node/BranchNode.hpp"

#include <gog43/Logger.hpp>

namespace mgc {
  SeedPathIterator::SeedPathIterator() {
    path_cur = -1;
    current_path = nullptr;

    jump = false;
  }

  SeedPathIterator::SeedPathIterator(PathNode* root_path) {
    current_path = root_path;

    if (root_path != nullptr) {
      path_cur = 0;
      point = root_path->at(0);
    } else {
      path_cur = -1;
    }

    jump = false;
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

  SeedPathIterator::reference SeedPathIterator::operator*() {
    return *this;
  }

  SeedPathIterator::pointer SeedPathIterator::operator->() {
    return this;
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
    jump = false;
    
    if (current_path != nullptr) {
      if (path_cur >= current_path->size()) {
        BranchNode* branch = current_path->next;
        // end of path - set cur back to 0
        path_cur = 0;

        if (branch != nullptr) {
          branch_stack.push(branch->main);
          current_path = branch->branch;
        } else {
          // end of branch
          if (!branch_stack.empty()) {
            // reached branch end, jumping back to main
            current_path = branch_stack.top();
            branch_stack.pop();

            // jumped from branch to last main - set jump to true
            jump = true;
            path_cur = 0;
          } else {
            // end of path!
            path_cur = -1;
            current_path = nullptr;
          }
        }
      }
    }

    if (current_path != nullptr) {
      // update currently stored point
      point = current_path->at(path_cur);
    }
  }
}