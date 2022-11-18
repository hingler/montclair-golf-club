#include <gtest/gtest.h>

#include "image/rgba_color.hpp"

using namespace image;

TEST(ColorSchemaTest, SimpleColorConversion) {
  rgba_color c;
  memset(&c, 0, sizeof(rgba_color));

  c.r = 240;
  c.g = 16;
  c.b = 64;
  c.a = 128;

  rgba_color out;

  uint32_t intermediate = rgba_color::rgba_to_color(c);
  rgba_color::color_to_rgba(intermediate, out);

  EXPECT_EQ(c.r, out.r);
  EXPECT_EQ(c.g, out.g);
  EXPECT_EQ(c.b, out.b);
  EXPECT_EQ(c.a, out.a);
}