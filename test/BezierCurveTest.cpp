#include <gtest/gtest.h>
#include <iostream>

#include "course/path/BezierCurve.hpp"

using namespace course;
using namespace path;

TEST(BezierCurveTest, EnsureSamplesRemainConsistent) {
  BezierCurve curve(glm::vec2(0, 0), glm::vec2(50, 50), glm::vec2(150, 50), glm::vec2(200, 0));
  glm::vec2 current_point = glm::vec2(0, 0);
  glm::vec2 last_point = glm::vec2(0, 0);

  double length = curve.Length();

  const double step = 0.0005;
  const double expected_step = length * step;
  
  for (double t = step; t < 1.0; t += step) {
    std::cout << t << std::endl;
    current_point = curve.Sample(t);
    double dist_covered = glm::length(current_point - last_point);
    EXPECT_NEAR(dist_covered, expected_step, 0.001);
    last_point = current_point;
  }
}

TEST(BezierCurveTest, VerifyBoundingBoxIsAccurate) {
  // TODO: :3
}