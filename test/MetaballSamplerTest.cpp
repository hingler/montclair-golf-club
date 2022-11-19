#include <gtest/gtest.h>

#include "course/sampler/MetaballSampler.hpp"

using namespace course;
using namespace sampler;

TEST(MetaballSamplerTest, SingleMetaball) {
  MetaballSampler sampler;
  sampler.AddMetaball(0.0f, 0.0f, 1.0f);

  EXPECT_NEAR(sampler.Sample(0.0f, 0.0f), 1.0f, 0.0001f);
  EXPECT_NEAR(sampler.Sample(0.5f, 0.5f), 1.0f, 0.0001f);
  EXPECT_NEAR(sampler.Sample(0.8f, 0.8f), 0.0f, 0.0001f);

  sampler.threshold = 2.5f;

  EXPECT_NEAR(sampler.Sample(0.5f, 0.0f), 0.0f, 0.0001f);
  EXPECT_NEAR(sampler.Sample(0.25f, 0.25f), 1.0f, 0.0001f);

  sampler.threshold = 0.24f;

  EXPECT_NEAR(sampler.Sample(4.0f, 0.0f), 1.0f, 0.0001f);
  EXPECT_NEAR(sampler.Sample(3.1f, 3.1f), 0.0f, 0.0001f);
}