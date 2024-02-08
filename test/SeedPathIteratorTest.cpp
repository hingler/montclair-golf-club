#include <gtest/gtest.h>

#include <gog43/Logger.hpp>
#include <gog43/IOStreamLogger.hpp>

#include "seed/path/SeedPath.hpp"
#include "seed/path/impl/SeedPathIterator.hpp"
#include "seed/path/SeedPathBuilder.hpp"

using namespace gog43;
using namespace terraingen;


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
    ASSERT_EQ(point.x, point.y);
  }

  auto itr = ptr->begin();

  int cur = 0;
  while (itr != ptr->end()) {


    glm::dvec2 pt = *itr;
    ASSERT_NEAR(pt.x, cur, 0.0001);
    ASSERT_NEAR(pt.y, cur, 0.0001);
    ++itr;
    ++cur;
  }
  ASSERT_EQ(cur, 32);

  print("test completed!");
}