#include <gtest/gtest.h>
#include <cmath>

#include "course/terrain_data.hpp"

using namespace course;

TEST(CourseSchemaTest, TestNormalizeFunc) {
  terrain_data s;
  memset(&s, 0, sizeof(terrain_data));
  s.terrain[0] = 2.5;
  s.terrain[1] = 2.5;
  s.terrain[2] = 2.5;

  s.normalize();

  float expected_value = 1.0f / 3.0f;
  EXPECT_NEAR(s.terrain[0], expected_value, 0.001);
  EXPECT_NEAR(s.terrain[1], expected_value, 0.001);
  EXPECT_NEAR(s.terrain[2], expected_value, 0.001);
}

TEST(CourseSchemaTest, TestPowerFunc) {
  terrain_data s;
  memset(&s, 0, sizeof(terrain_data));
  s.terrain[terrain_type::Fairway] = 2.0;
  s.terrain[terrain_type::Green] = 4.0;
  s.terrain[terrain_type::Sand] = 8.0;

  s.power(2.0f);

  EXPECT_NEAR(s.terrain[terrain_type::Fairway], 4.0, 0.001);
  EXPECT_NEAR(s.terrain[terrain_type::Green], 16.0, 0.001);
  EXPECT_NEAR(s.terrain[terrain_type::Sand], 64.0, 0.001);
  EXPECT_NEAR(s.terrain[Rough], 0.0, 0.001);
}

// sized container makes sense
// but it would also be nice if we could iterate simply instead of dealing with the pointer terrain[terrain_type::Rough]