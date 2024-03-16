#include "seed/hole/impl/SimplePathParser.hpp"
#include "seed/path/node/PathNode.hpp"
#include "seed/path/node/BranchNode.hpp"

#include <gog43/Logger.hpp>

namespace mgc {
  namespace pathparser {
    static void parse_recurse(const mgc::PathNode* node, std::vector<const PathNode*>& output) {
      const PathNode* cur = node;

      while (cur != nullptr) {
        output.push_back(cur);
        const BranchNode* next = cur->next;
        if (next != nullptr) {
          cur = next->main;
          if (next->branch != nullptr) {
            parse_recurse(next->branch, output);
          }
        } else {
          cur = nullptr;
        }
      }

    }

    std::vector<const PathNode*> parse(const std::vector<std::shared_ptr<SeedPath>>& paths) {
      std::vector<const PathNode*> result;
      for (const auto& path : paths) {
        parse_recurse(path->GetRoot(), result);
      }

      return result;
    }
  }
}
