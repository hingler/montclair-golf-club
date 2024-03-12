#include <gtest/gtest.h>

#include <gog43/Logger.hpp>
#include <gog43/IOStreamLogger.hpp>

#include <glm/glm.hpp>

#include "seed/hole/HoleBox.hpp"
#include "seed/hole/HoleChunkBox.hpp"

class HoleChunkBoxTest : public ::testing::Test {
 protected:
  void SetUp() {
    gog43::provide_logger(new gog43::IOStreamLogger());
  }
};

using namespace mgc;

static HoleBox get(const glm::dvec2& origin, const glm::dvec2& size, double padding) {
  std::vector<glm::dvec2> points;
  points.push_back(origin);
  points.push_back(origin + size);
  return HoleBox(points, padding, 1);
}

TEST_F(HoleChunkBoxTest, BasicTest) {
  std::vector<HoleBox> holes;
  holes.push_back(get(glm::dvec2(0), glm::dvec2(32), 0.0));
  holes.push_back(get(glm::dvec2(-16), glm::dvec2(40), 0.0));

  HoleChunkBox box(holes, glm::ivec2(0));

  glm::dvec2 origin = box.GetOrigin();
  glm::dvec2    end = box.GetEnd();

  ASSERT_NEAR(origin.x, -16.0, 0.0001);
  ASSERT_NEAR(origin.y, -16.0, 0.0001);

  ASSERT_NEAR(end.x, 32.0, 0.0001);
  ASSERT_NEAR(end.y, 32.0, 0.0001);
}

TEST_F(HoleChunkBoxTest, UnevenTest) {
  std::vector<HoleBox> holes;

  holes.push_back(get(glm::dvec2(-15, 22), glm::dvec2(22, 44), 16.0));
  holes.push_back(get(glm::dvec2(37, 21), glm::dvec2(15, 21), 4.0));
  holes.push_back(get(glm::dvec2(81, 22), glm::dvec2(44, 22), 0.0));

  HoleChunkBox box(holes, glm::ivec2(0, 0));

  glm::dvec2 origin = box.GetOrigin();
  glm::dvec2    end = box.GetEnd();

  ASSERT_NEAR(origin.x, -31.0, 0.0001);
  ASSERT_NEAR(origin.y, 6.0, 0.0001);

  ASSERT_NEAR(end.x, 125.0, 0.0001);
  ASSERT_NEAR(end.y, 82.0, 0.0001);
}