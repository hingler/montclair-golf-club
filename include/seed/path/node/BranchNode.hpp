#ifndef BRANCH_NODE_H_
#define BRANCH_NODE_H_

#include "seed/path/node/PathNode.hpp"

// no risk in revisiting - use same init. point for both

// how to build?
// start w an empty path node (pass in a ptr to it)
// step along, adding points
// on branch:
// - create branch node in place
// - create a branch pathnode and pass ptr to another builder
// - create a main pathnode and start building off it

// maintain a root ptr across all copies so we can get the init tree

// root ptr
// maintain a shared "root ptr" which is initialized with a path node
// root ptr maintains memory
// we fetch a shared ref to the root ptr to march the path
// root ptr manages itr etc (ie seedpath)

namespace terraingen {
  struct BranchNode {
    PathNode* main;
    PathNode* branch;
  };
}

#endif // BRANCH_NODE_H_