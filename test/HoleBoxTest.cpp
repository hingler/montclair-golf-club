#include <gtest/gtest.h>

#include "seed/hole/HoleBox.hpp"

#include "gog43/Logger.hpp"
#include "gog43/IOStreamLogger.hpp"

class HoleBoxTest : public ::testing::Test {
 protected:
  void SetUp() override {
    gog43::provide_logger(new gog43::IOStreamLogger());
  }
};

using namespace mgc;

TEST_F(HoleBoxTest, CtorAndBoundsTest) {
  std::vector<glm::dvec2> points;
  for (size_t i = 0; i <= 32; i++) {
    points.push_back(glm::dvec2(i, i));
  }

  HoleBox box(points, 0, 1);
  glm::dvec2 origin = box.GetOrigin();
  glm::dvec2 end    = box.GetEnd();
  ASSERT_NEAR(0.0, origin.x, 0.0001);
  ASSERT_NEAR(0.0, origin.y, 0.0001);

  ASSERT_NEAR(32.0, end.x, 0.0001);
  ASSERT_NEAR(32.0, end.y, 0.0001);

  // add padding

  HoleBox padded(points, 32, 1);
  origin = padded.GetOrigin();
  end    = padded.GetEnd();
  ASSERT_NEAR(-32.0, origin.x, 0.0001);
  ASSERT_NEAR(-32.0, origin.y, 0.0001);

  ASSERT_NEAR(64.0, end.x, 0.0001);
  ASSERT_NEAR(64.0, end.y, 0.0001);
}

TEST_F(HoleBoxTest, PathTest) {
  PathNode node(glm::dvec2(0));
  for (int i = 0; i <= 64; i++) {
    node.append(glm::dvec2(i - 32, i));
  }

  gog43::print("size: ", node.size());

  node.next = nullptr;

  HoleBox from_path(&node, 0.0, 5489675489);
  glm::dvec2 origin = from_path.GetOrigin();
  glm::dvec2    end = from_path.GetEnd();

  ASSERT_NEAR(-32.0, origin.x, 0.0001);
  ASSERT_NEAR(0.0, origin.y, 0.0001);

  ASSERT_NEAR(32.0, end.x, 0.0001);
  ASSERT_NEAR(64.0, end.y, 0.0001);
}
