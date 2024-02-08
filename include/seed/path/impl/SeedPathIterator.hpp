#ifndef SEED_PATH_ITERATOR_H_
#define SEED_PATH_ITERATOR_H_

#include "seed/path/node/PathNode.hpp"

#include <iterator>
#include <stack>

#include <glm/glm.hpp>

namespace terraingen {
  class SeedPathIterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    using value_type = glm::dvec2;
    using reference = value_type&;
    using pointer = value_type*;

    SeedPathIterator();
    SeedPathIterator(PathNode* root_path);
    
    SeedPathIterator& operator++();
    SeedPathIterator operator++(int);

    value_type operator*();
    // operator-> ?

    bool operator==(const SeedPathIterator& other) const;
    bool operator!=(const SeedPathIterator& other) const;

    
   private:
    void update_path();
    // path node stack (on branch: stack main, visit branch)
    // cursor (progress through stack)

    int path_cur;
    const PathNode* current_path;
    std::stack<PathNode*> branch_stack;
  };
}

#endif // SEED_PATH_ITERATOR_H_