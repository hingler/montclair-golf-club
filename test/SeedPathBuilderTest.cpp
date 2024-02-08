#include <gtest/gtest.h>

#include "gog43/Logger.hpp"
#include "gog43/IOStreamLogger.hpp"

#include "seed/path/SeedPathBuilder.hpp"

#include "seed/path/node/BranchNode.hpp"

using namespace gog43;
using namespace terraingen;


class SeedPathBuilderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    provide_logger(new IOStreamLogger());
  }
};

TEST_F(SeedPathBuilderTest, SimpleTest) {
  print("holey moley!");

  SeedPathBuilder builder(glm::dvec2(0, 0));
  for (int i = 1; i < 64; i++) {
    builder.AddPoint(glm::dvec2(i, i));
  }

  std::shared_ptr<SeedPath> path = builder.Build();
  const PathNode* root = path->GetRoot();
  for (int i = 0; i < 64; i++) {
    ASSERT_NEAR(root->get(i).x, i, 0.0001);
    ASSERT_NEAR(root->get(i).y, i, 0.0001);
  }

  print(root->size());
}

TEST_F(SeedPathBuilderTest, VerifyBranch) {
  SeedPathBuilder builder(glm::dvec2(32, 32));

  // 32 -> 48
  for (int i = 1; i <= 16; i++) {
    builder.AddPoint(glm::dvec2(32 + i));
  }
  
  SeedPathBuilder branch = builder.AddBranch();

  for (int i = 1; i < 16; i++) {
    branch.AddPoint(glm::dvec2(48 - i, 48 + i));
    builder.AddPoint(glm::dvec2(48 + i));
  }

  std::shared_ptr<SeedPath> res = builder.Build();

  const PathNode* root = res->GetRoot();
  const BranchNode* branch_node = root->next;

  print(root->size());
  ASSERT_NE(branch_node, nullptr);

  const PathNode* branch_path = branch_node->branch;
  const PathNode* main_path = branch_node->main;

  for (int i = 0; i < 16; i++) {
    glm::dvec2 pt_main = root->get(i);
    ASSERT_NEAR(pt_main.x, i + 32, 0.0001);
    ASSERT_NEAR(pt_main.y, i + 32, 0.0001);
  }

  for (int i = 0; i < 16; i++) {
    glm::dvec2 pt_main = main_path->get(i);
    glm::dvec2 pt_branch = branch_path->get(i);

    ASSERT_NEAR(pt_main.x, 48 + i, 0.0001);
    ASSERT_NEAR(pt_main.y, 48 + i, 0.0001);

    ASSERT_NEAR(pt_branch.x, 48 - i, 0.0001);
    ASSERT_NEAR(pt_branch.y, 48 + i, 0.0001);
  }

  print("swaus");
}