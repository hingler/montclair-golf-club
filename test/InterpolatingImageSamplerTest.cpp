#include <gtest/gtest.h>

#include "image/GenericImage.hpp"
#include "course/sampler/InterpolatingImageSampler.hpp"

using namespace image;
using namespace course;
using namespace sampler;

struct test_data {
  float data;

  test_data& operator+=(const test_data& rhs) {
    data += rhs.data;
    return *this;
  }

  test_data& operator*=(const float rhs) {
    data *= rhs;
    return *this;
  }
};

test_data operator+(test_data lhs, const test_data& rhs) {
  lhs.data += rhs.data;
  return lhs;
}

test_data operator*(test_data lhs, float rhs) {
  lhs.data *= rhs;
  return lhs;
}

TEST(InterpolatingImageSamplerTest, TestSimpleSample) {
  std::shared_ptr<GenericImage<test_data>> image = std::make_shared<GenericImage<test_data>>(32, 32);
  test_data data;
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      data.data = j * 32.0 + static_cast<float>(i);
      image->Set(i, j, data);
    }
  }

  InterpolatingImageSampler<test_data> sampler(image, glm::vec2(64.0, 64.0));

  auto sample = sampler.Sample(1.0, 1.0);
  EXPECT_NEAR(sample.data, 0.0, 0.001);

  sample = sampler.Sample(3.0, 3.0);
  EXPECT_NEAR(sample.data, 33.0, 0.001);

  // clip sampling should read bottom left pixel only
  sample = sampler.Sample(-4.0, 0.0);
  EXPECT_NEAR(sample.data, 0.0, 0.001);

  sample = sampler.Sample(33.0, 33.0);
  EXPECT_NEAR(sample.data, 528.0, 0.001);

  // interpolate
  sample = sampler.Sample(2.0, 1.0);
  EXPECT_NEAR(sample.data, 0.5, 0.001);

  sample = sampler.Sample(68.0, 75.0);
  EXPECT_NEAR(sample.data, 1023.0, 0.001);
  
  sample = sampler.Sample(32.0, 75.0);
  EXPECT_NEAR(sample.data, 1007.5, 0.001);
}