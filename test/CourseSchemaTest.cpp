#include <gtest/gtest.h>
#include <cmath>

#include "course/course_schema.hpp"

using namespace course;

TEST(CourseSchemaTest, TestNormalizeFunc) {
  course_schema s;
  memset(&s, 0, sizeof(course_schema));
  s.terrain[terrain_type::Fairway] = 2.5;
  s.terrain[terrain_type::Green] = 2.5;
  s.terrain[terrain_type::Rough] = 2.5;

  s.normalize();

  float expected_value = sqrt(1.0f / 3.0f);
  EXPECT_NEAR(s.terrain[terrain_type::Fairway], expected_value, 0.001);
  EXPECT_NEAR(s.terrain[terrain_type::Green], expected_value, 0.001);
  EXPECT_NEAR(s.terrain[terrain_type::Rough], expected_value, 0.001);
}

TEST(CourseSchemaTest, TestPowerFunc) {
  course_schema s;
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