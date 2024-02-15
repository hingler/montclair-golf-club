#include <gtest/gtest.h>

#include "gog43/Logger.hpp"
#include "gog43/IOStreamLogger.hpp"

#include "debug/draw/SeedPathDraw.hpp"
#include "seed/path/SeedPathBuilder.hpp"

using namespace gog43;
using namespace mgc;

class SeedPathDrawTest : public ::testing::Test {
 protected:
  void SetUp() override {
    provide_logger(new IOStreamLogger());
  }
};

glm::dvec2 point_func(int i) {
  if (i == 0) {
    return glm::dvec2(-15, 15);
  }

  // starts in -x, y
  // spins out in the (-x, y) direction

  // ergo: y, x will be in the x, -y quadrant - helpful for testing
  return glm::dvec2(i + sin(i) * -7.21 - 17, i + cos(i) * 14.216 + 31);
}

TEST_F(SeedPathDrawTest, BabyTest) {
  SeedPathBuilder b(glm::dvec2(-15, 15));
  for (int i = 1; i < 32; i++) {
    b.AddPoint(point_func(i));
  }

  SeedPathDraw draw(b.Build(), 5.0);

  for (double i = 0.0; i < 31.9; i += 0.216) {
    glm::dvec2 point_init = point_func((int)i);
    glm::dvec2 point_ceil = point_func((int)i + 1);
    double mix = glm::fract(i);

    glm::dvec2 point_expected = point_init * (1.0 - mix) + point_ceil * (mix);

    ASSERT_NEAR(draw.Sample(point_expected.x, point_expected.y), 1.0, 0.00001);
    ASSERT_NEAR(draw.Sample(-point_expected.x, -point_expected.y), 0.0, 0.00001);
  }


}

TEST_F(SeedPathDrawTest, StressTest) {
  SeedPathBuilder b(glm::dvec2(0, 0));
  for (double i = 0; i < 256.1414; i += 0.45) {
    b.AddPoint(glm::dvec2(sin(i) * 1.414 * i, cos(i) * 2.114 * i));
  }

  SeedPathDraw draw(b.Build(), 5.0);

  double m = 0.0;

  for (double i = -128; i < 128; i += 0.45) {
    for (double j = -128; j < 128; j += 0.43) {
      m += draw.Sample(i, j);
    }
  }

  print(m);
}

TEST_F(SeedPathDrawTest, JumpTest) {
  SeedPathBuilder b(glm::dvec2(0));
  b.AddPoint(glm::dvec2(25, 25));

  SeedPathBuilder branch = b.AddBranch();

  branch.AddPoint(glm::dvec2(50, 40));
  branch.AddPoint(glm::dvec2(25, 50));
  // will jump back to 25, 25

  b.AddPoint(glm::dvec2(0, 25));

  SeedPathDraw draw(b.Build(), 5.0);

  ASSERT_EQ(draw.Sample(37.5, 32.5), 1.0);
  ASSERT_EQ(draw.Sample(37.5, 45), 1.0);

  // btwn end of branch and start of branch - on jump, will pass thru this
  ASSERT_EQ(draw.Sample(25, 32), 0.0);
}

// work on seed wander
// - i think "rolling" and "root travel" shuold be separate
// - plop down a thing and let it do its thing