#include <gtest/gtest.h>

#include "course/sampler/SimplexNoiseSampler.hpp"

using namespace course;
using namespace sampler;

TEST(SimplexNoiseSampler, MultiOctaveRangeTest) {
  SimplexNoiseSampler sampler(glm::vec3(1.0), 9);
  for (float i = -256.0; i < 256.0; i += 0.139140185) {
    for (float j = -256.0; j < 256.0; j += 0.08775192) {
      ASSERT_LE(sampler.Sample(i, j), 1.0);
      ASSERT_GE(sampler.Sample(i, j), -1.0);
    }
  }
}