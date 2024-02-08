#include "seed/path/SeedPathBuilder.hpp"

#include "seed/path/node/BranchNode.hpp"

namespace terraingen {

  SeedPathBuilder::SeedPathBuilder(const glm::dvec2& init_point) 
  : underlying(std::make_shared<SeedPathImpl>(init_point)) 
  {
    current_node = underlying->GetRoot_write();
  }

  SeedPathBuilder::SeedPathBuilder(const std::shared_ptr<SeedPathImpl>& root, PathNode* init_path) 
  : underlying(root), current_node(init_path) 
  {
    // nop
  }

  std::shared_ptr<SeedPath> SeedPathBuilder::Build() {
    return std::dynamic_pointer_cast<SeedPath>(underlying);
  }

  void SeedPathBuilder::AddPoint(const glm::dvec2& point) {
    current_node->append(point);
  }

  SeedPathBuilder SeedPathBuilder::AddBranch() {
    // how else can we do this???
    // - return the copy
    BranchNode* res = new BranchNode();

    glm::dvec2 branch_point = current_node->at(current_node->size() - 1);
    current_node->next = res;
    
    PathNode* main_path = new PathNode(branch_point);
    PathNode* branch_path = new PathNode(branch_point);
    current_node = main_path;

    res->main = main_path;
    res->branch = branch_path;

    return SeedPathBuilder(underlying, branch_path);
  }
}