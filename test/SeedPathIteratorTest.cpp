#include <gtest/gtest.h>

#include <gog43/Logger.hpp>
#include <gog43/IOStreamLogger.hpp>

#include "seed/path/SeedPath.hpp"
#include "seed/path/impl/SeedPathIterator.hpp"
#include "seed/path/SeedPathBuilder.hpp"

using namespace gog43;
using namespace mgc;


class SeedPathIteratorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    provide_logger(new IOStreamLogger());
  }
};

TEST_F(SeedPathIteratorTest, SimpleEndEqualityTest) {
  SeedPathIterator a, b;
  std::vector<float> test;
  print("g!");
  ASSERT_EQ(a, b);
}

TEST_F(SeedPathIteratorTest, SimplePathStepTest) {
  SeedPathBuilder b(glm::dvec2(0));
  for (int i = 1; i < 32; i++) {
    b.AddPoint(glm::dvec2(i));
  }

  auto ptr = b.Build();
  ASSERT_EQ(32, ptr->GetRoot()->size());

  for (const auto point : *ptr) {
    ASSERT_EQ(point.point.x, point.point.y);
  }

  auto itr = ptr->begin();

  int cur = 0;
  while (itr != ptr->end()) {


    glm::dvec2 pt = itr->point;
    ASSERT_NEAR(pt.x, cur, 0.0001);
    ASSERT_NEAR(pt.y, cur, 0.0001);
    ++itr;
    ++cur;
  }
  ASSERT_EQ(cur, 32);

  print("test completed!");
}

TEST_F(SeedPathIteratorTest, BranchingIteratorTest) {
  SeedPathBuilder b(glm::dvec2(0));
  for (int i = 1; i <= 32; i++) {
    b.AddPoint(glm::dvec2(i));
  }

  SeedPathBuilder branch = b.AddBranch();

  for (int i = 1; i <= 16; i++) {
    branch.AddPoint(glm::dvec2(32 + i, 32 + i));
    b.AddPoint(glm::dvec2(32 + i, 32 - i));
  }

  auto ptr = b.Build();
  ASSERT_EQ(33, ptr->GetRoot()->size()); 

  print("visiting main");
  auto itr = ptr->begin();
  for (int i = 0; i <= 32; i++) {
    ASSERT_NEAR(itr->point.x, i, 0.0001);
    ASSERT_NEAR(itr->point.y, i, 0.0001);

    ++itr;
  }

  // not a jump - just following branch
  ASSERT_FALSE(itr->jump);

  // visit branch next
  print("visiting branch");
  for (int i = 0; i <= 16; i++) {
    ASSERT_NEAR(itr->point.x, 32 + i, 0.0001);
    ASSERT_NEAR(itr->point.y, 32 + i, 0.0001);

    ++itr; 
  }

  // branch ended, ensure that we record a jump back to main
  ASSERT_TRUE(itr->jump);
  
  // jump back to the start of the main path
  print("back to main");
  for (int i = 0; i <= 16; i++) {
    ASSERT_NEAR(itr->point.x, 32 + i, 0.0001);
    ASSERT_NEAR(itr->point.y, 32 - i, 0.0001);

    ++itr;
  }
}

// branching works - what next?

//  - want a little debug sampler for the path (not sure how best to accomplish)
//    - recurse over path nodes, and create line segments
//    - just do a dist check on those segments - not great, but not terrible
// - thinking: i'd like to write a more convenient debugger component
//    - manager-like - just add sampler templates <virtual wrap>
//    - ...and also assign them colors
//    - blend the whole thing together