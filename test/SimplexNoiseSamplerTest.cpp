#include <gtest/gtest.h>

#include "course/sampler/SimplexNoiseSampler.hpp"

#define TOLERANCE 0.55

#define STEP_X 0.2592183
#define STEP_Y 0.19391231

using namespace course;
using namespace sampler;

TEST(SimplexNoiseSampler, MultiOctaveRangeTest) {
  SimplexNoiseSampler sampler(glm::vec3(6.0), 9);
  // 5120 x 5120 x 9
  // 225 mil samples
  // 10s
  // 20mil samples / s per thread :( [might haveto figure out how to do better]
  for (float i = -256.0; i < 256.0; i += STEP_X) {
    for (float j = -256.0; j < 256.0; j += STEP_Y) {
      float res = sampler.Sample(i, j);
      ASSERT_LE(res, TOLERANCE);
      ASSERT_GE(res, -TOLERANCE);
    }
  }
}