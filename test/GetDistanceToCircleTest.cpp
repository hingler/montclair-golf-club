#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include "course/generator/impl/GetDistanceToCircle.hpp"

using namespace course;
using namespace generator;
using namespace impl;

TEST(GetDistanceToCircleTest, TestRayFromCenter) {
  float radius = 100.0f;

  for (float i = 0; i < 6.28; i += 0.06) {
    glm::vec2 dir(cos(i), sin(i));
    float res = GetDistanceToCircle(glm::vec2(0, 0), dir, glm::vec2(0, 0), radius);
    EXPECT_NEAR(res, radius, 0.01);
  }
}

TEST(GetDistanceToCircleTest, SanityTest) {
  // pick a random point inside the circle
  // pick a random direction
  // expect result to be greater than the min dist (thru point) and less than diam
  const float radius = 125.0f;

  const float max_dist = 2 * radius;
  for (float theta = 0; theta < 640.0; theta += 2.6124) {
    for (float r = 15.0f - sin(theta) * 12.5; r < radius; r += 25.121) {
      float min_dist = (radius - r);
      glm::vec2 pos(cos(theta) * r, sin(theta) * r);

      for (float theta_dir = 0.16023f; theta_dir < 6.28; theta_dir += 0.7) {
        glm::vec2 dir(cos(theta_dir), sin(theta_dir));
        float dist = GetDistanceToCircle(pos, dir, glm::vec2(0, 0), radius);
        EXPECT_LE(dist, max_dist);
        EXPECT_GE(dist, min_dist);
        
        glm::vec2 result = pos + dir * dist;
        float dist_to_center = glm::length(result);
        EXPECT_NEAR(dist_to_center, radius, 0.01);
      }
    }
  }
}