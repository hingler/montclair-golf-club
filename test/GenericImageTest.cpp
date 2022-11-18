#include <gtest/gtest.h>

#include "image/GenericImage.hpp"

using namespace image;

struct test_data {
  uint32_t image_data;
};

TEST(GenericImageTest, TestWriteAndRead) {
  GenericImage<test_data> image(32, 32);
  test_data input;
  input.image_data = 4096;
  image.Set(16, 16, input);

  auto* val = image.Get(16, 16);
  EXPECT_NE(val, nullptr);
  EXPECT_EQ(val->image_data, 4096);

  image.Set(33, 33, input);
  auto* val_oob = image.Get(33, 33);
  EXPECT_EQ(val_oob, nullptr);
}

TEST(GenericImageTest, TestCopyCtor) {
  GenericImage<test_data> image(32, 32);
  test_data input;
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      input.image_data = i * 32 + j;
      image.Set(i, j, input);
    }
  }

  GenericImage<test_data> copy(image);
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      uint32_t expected = i * 32 + j;
      auto* val = copy.Get(i, j);
      EXPECT_EQ(expected, val->image_data);
    }
  }
}