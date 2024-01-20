#define _USE_MATH_DEFINES

#include "course/generator/BruteForceCourseGenerator.hpp"
#include "course/generator/impl/GetDistanceToCircle.hpp"


#include <algorithm>
#include <iostream>

#include <cmath>

namespace course {

    using namespace sampler;

  namespace generator {
    using namespace path;

    BruteForceCourseGenerator::BruteForceCourseGenerator() : simple_dist(0.0, 1.0), binary_dist(0, 1) { }

    CoursePath BruteForceCourseGenerator::GenerateCourse(std::shared_ptr<ISampler<float>> terrain_in, const glm::vec2& terrain_size_in) {
      // prepare instance vars
      terrain = terrain_in;
      terrain_size = terrain_size_in;
      engine.seed(this->seed);

      CoursePath res;
      res.course_path.clear();

      // pick some tee location
      for (int i = 0; i < 64; i++) {
        tee_point = GenerateTeeLocation(terrain, terrain_size);

        // generate from tee location
        auto course = GenerateFromTee();
        std::cout << course.shots << std::endl;
        if (course.shots > 0) {
          res.course_path.resize(course.shots);
          while (course.shots > 0) {
            int shots = course.shots;
            res.course_path[shots - 1] = course.PopPosition();
            std::cout << res.course_path[shots - 1].x << ", " << res.course_path[shots - 1].y << std::endl;
          }

          return res;
        }
      }
      return CoursePath();
    }

    impl::PositionTracker BruteForceCourseGenerator::GenerateFromTee() {
      // calculate shots to green
      int shots_to_green = 1;
      std::uniform_real_distribution<float>par_nudge(-10.0f, 10.0f);
      float calculation_yardage = yardage + par_nudge(engine);

      // determine how many shots we have to take
      if (calculation_yardage > 225) {
        shots_to_green++;
      }

      if (calculation_yardage > 485) {
        shots_to_green++;
      }

      // to reuse point code:
      // generate a dummy "previous point" away from our center
      glm::vec2 dummy_prev = tee_point - (terrain_size / 2.0f - tee_point);

      auto dir = glm::normalize(tee_point - dummy_prev);
      impl::PositionTracker seed_final;
      seed_final.PushPosition(tee_point);
      float res = GenerateFromPoint(
        tee_point,
        dummy_prev,
        shots_to_green,
        7,
        seed_final);

      if (res < 0.5f) {
        seed_final.PopPosition();
      }
      // it'll be empty if we failed :(
      return seed_final;
    }

    float BruteForceCourseGenerator::GenerateFromPoint(
      const glm::vec2& current_point,
      const glm::vec2& previous_point,
      int shots_remaining,
      int sample_depth,
      impl::PositionTracker& output
    ) {
      float distance_remaining = yardage - glm::length(current_point - tee_point);
      if (shots_remaining <= 0 || (distance_remaining < 0.5f && distance_remaining > -0.5f)) {
        // return 1 if we're pretty much there else 0
        return (distance_remaining < 0.5f ? 1.0f : 0.0f);
      }
      
      // above catches tee case
      // we're either too far, or we overshot
      if ((shots_remaining <= 1 && distance_remaining > 270.0f) || (shots_remaining > 1 && distance_remaining <= -0.5f)) {
        return 0.0f;
      }

      int sample_count = (1 << sample_depth) - 1;
      for (int i = 0; i < sample_count; i++) {
        // square sample, to prefer down the middle
        double theta = simple_dist(engine);
        // bias towards the middle
        theta = pow(theta, 1.4);
        // flip along main axis
        theta *= (binary_dist(engine) == 1 ? 1 : -1) * M_PI_2;
        glm::vec2 main_axis = glm::normalize(current_point - previous_point);
        glm::vec2 cross_axis = glm::vec2(main_axis.y, -main_axis.x);
        // figure out which direction our shot is going in
        glm::vec2 travel_direction = glm::normalize(main_axis * static_cast<float>(cos(theta)) + cross_axis * static_cast<float>(sin(theta)));
        float shot_distance;
        if (shots_remaining > 1 && distance_remaining > 280.0f) {
          // take a shot!
          shot_distance = simple_dist(engine) * 35.0f + 220.0f;
        } else {
          // we're pretty much at the hole
          shot_distance = impl::GetDistanceToCircle(current_point, travel_direction, tee_point, yardage);
          if (shot_distance > 280.0f) {
            // one shot left, too long
            continue;
          }
        }

        glm::vec2 next_point = current_point + travel_direction * shot_distance;
        // if next point is too far off the map, skip
        if (next_point.x < terrain_size.x * 0.1 || next_point.x > terrain_size.x * 0.9 || next_point.y < terrain_size.y * 0.1 || next_point.y > terrain_size.y * 0.9) {
          // too close to map border
          continue;
        }
        // we've found a candidate point
        // we push children
        output.PushPosition(next_point);

        // step down the chain from there, taking one shot
        float generation_result = GenerateFromPoint(
          next_point,
          current_point,
          shots_remaining - 1,
          sample_depth,
          output
        );

        if (generation_result < 0.1f) {
          // generation failed, back up
          output.PopPosition();
        } else {
          // put our score here later
          return 1.0f * generation_result;
        }
      }

      // no good sample points :(
      return 0.0f;
    }

    glm::vec2 BruteForceCourseGenerator::GenerateTeeLocation(std::shared_ptr<sampler::ISampler<float>> terrain, const glm::vec2& terrain_size) {
      std::uniform_real_distribution<double> radius_dist(0.0, 1.0);
      std::uniform_real_distribution<double> angle_dist(0.0, 2 * M_PI);

      for (int i = 0; i < 512; i++) {
        double theta = angle_dist(engine);
        double r = radius_dist(engine);

        // normally we'd prune here or something
        glm::vec2 center(terrain_size.x / 2, terrain_size.y / 2);
        float max_radius = std::min(center.x, center.y) * 0.8;
        r = sqrt(r);
        
        return glm::vec2(center.x + cos(theta) * r * max_radius, center.y + sin(theta) * r * max_radius);
      }

      return glm::vec2(0, 0);
    }
  }
}