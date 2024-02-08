#include <gtest/gtest.h>

#include "course/path/CompoundCurve.hpp"
#include "course/path/BezierCurve.hpp"

#include <glm/glm.hpp>

#include <memory>

using namespace course;
using namespace path;

TEST(CompoundCurveTest, SingleCurveTest) {
  std::shared_ptr<BezierCurve> curve = std::make_shared<BezierCurve>(glm::vec2(0, 0), glm::vec2(50, 50), glm::vec2(150, 50), glm::vec2(200, 0));
  CompoundCurve compound;
  compound.AddSegment(curve);


  glm::vec2 current_point = glm::vec2(0, 0);
  glm::vec2 last_point = glm::vec2(0, 0);

  glm::vec2 reference_point;

  double length = compound.Length();

  const double step = 0.0005;
  const double expected_step = length * step;
  
  for (double t = step; t < 1.0; t += step) {
    current_point = compound.Sample(t);
    reference_point = curve->Sample(t);

    EXPECT_NEAR(current_point.x, reference_point.x, 0.0001);
    EXPECT_NEAR(current_point.y, reference_point.y, 0.0001);
    double dist_covered = glm::length(current_point - last_point);
    EXPECT_NEAR(dist_covered, expected_step, 0.001);
    last_point = current_point;
  }
}

TEST(CompoundCurveTest, MultiCurveTest) {
  std::shared_ptr<BezierCurve> curve = std::make_shared<BezierCurve>(glm::vec2(0, 0), glm::vec2(50, 50), glm::vec2(150, 50), glm::vec2(200, 0));
  // just resample it a few times
  CompoundCurve compound;
  compound.AddSegment(curve);
  compound.AddSegment(curve);
  compound.AddSegment(curve);

  glm::vec2 current_point = glm::vec2(0, 0);
  glm::vec2 last_point = glm::vec2(0, 0);

  glm::vec2 reference_point;

  double length = compound.Length();

  // 30000 steps :3
  const double step = 0.0001;
  const double expected_step = length * step;
  
  for (double t = step; t < 3.0; t += step) {
    current_point = compound.Sample(t * 0.3333333333333333);
    reference_point = curve->Sample(glm::mod(t, 1.0));

    // this should suffice for this
    EXPECT_NEAR(current_point.x, reference_point.x, 0.0001);
    EXPECT_NEAR(current_point.y, reference_point.y, 0.0001);
    last_point = current_point;
  }
}

TEST(CompoundCurveTest, VerifyBoundingBoxIsAccurate) {
  // TODO: :3
}