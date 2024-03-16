#include "path/deps/impl/SimpleControlPointCalculator.hpp"

#include <cmath>
#include <gtest/gtest.h>

#include <gog43/Logger.hpp>
#include <gog43/IOStreamLogger.hpp>

class SCPCTest : public ::testing::Test {
 protected:
  void SetUp() {
    gog43::provide_logger(new gog43::IOStreamLogger());
  }
};

using namespace mgc::deps::impl;

TEST_F(SCPCTest, ParTest) {
  SimpleControlPointCalculator c;
  ASSERT_EQ(c.GetPar(200), 3);
  ASSERT_EQ(c.GetPar(250), 4);

  ASSERT_EQ(c.GetPar(375), 4);
  ASSERT_EQ(c.GetPar(445), 5);

  ASSERT_EQ(c.GetPar(525), 5);
  ASSERT_EQ(c.GetPar(600), 5);
  ASSERT_EQ(c.GetPar(700), 6);
}

TEST_F(SCPCTest, ControlPointTest) {
  std::vector<glm::dvec2> point_list;
  for (int i = 0; i < 512; i++) {
    point_list.push_back(glm::dvec2(
      static_cast<double>(i),
      std::sin(i / 36.5) * 16.0
    ));
  }

  SimpleControlPointCalculator c;
  std::mt19937_64 engine;
  auto controls = c.GetControlPoints(point_list, engine);



  ASSERT_EQ(controls.at(0), 0);
  ASSERT_EQ(controls.at(0), 0);

  size_t cur_points =   0;
  size_t cur_controls = 0;

  gog43::print("control points: ", controls.size());

  while (cur_points < point_list.size() && cur_controls < controls.size()) {
    if (
      std::abs(point_list.at(cur_points).x - point_list.at(controls.at(cur_controls)).x) < 0.0001
      && std::abs(point_list.at(cur_points).y - point_list.at(controls.at(cur_controls)).y) < 0.0001
    ) {
      gog43::print("got it!");
      ++cur_controls;
    } else {
      ++cur_points;
    }
  }

  ASSERT_EQ(cur_controls, controls.size());

  ASSERT_EQ(controls.at(controls.size() - 1), point_list.size() - 1);
}
