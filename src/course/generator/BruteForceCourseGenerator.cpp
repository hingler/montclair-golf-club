#include "course/generator/BruteForceCourseGenerator.hpp"
#include "course/generator/impl/GetDistanceToCircle.hpp"


#include <algorithm>

namespace course {

    using namespace sampler;

  namespace generator {
    using namespace path;

    BruteForceCourseGenerator::BruteForceCourseGenerator() : simple_dist(0.0, 1.0), binary_dist(0, 1) { }

    impl::PositionTracker BruteForceCourseGenerator::GenerateCourse(std::shared_ptr<ISampler<float>> terrain_in, const glm::vec2& terrain_size_in) {
      // pick a tee location

      // from that tee location:
      // in each direction, generate a few random drives (on a par 3, it'll just be the one shot)
      // pass that point to `GenerateFromPoint`
      // it will handle the next shot

      // generate from point:
      // we generally want to favor straighter courses
      // concentrate samples down the middle, with fewer at increasing angles (we can control this probably)
      // fix distance around "average" shots with some deviation
      // deviation needs to be deterministic, so we can reconstruct these

      // i figure we split our bit depth 70/30
      // distance can be lower fidelity than angle
      // i think we should take a lot of "samples ahead" but only visit a few of them
      // "visit" (depth) samples
      // pick a smaller number which are "good" (16 max, lets say)
      // those we'll feel out

      // how do we pick a high deviation sequence?
      // just select randomly from our qualifying points and itll take care of itself :3
      // (how do we select randomly?)
      // presumably: we'll have some seedable random component that we can use
      // (q: thread safety???)
      // a: let's limit how often we fetch our seed i guess
      // sticking to the deterministic approach should limit how often we need to fetch the seed itself
      // note2: threading could ruin generation
      // it seeds go out of order, we're fucked!
      // don't worry about threading for now, we'll just trust the system :3

      // possibility: give each thread an "initial seed" (ie we split tee generation only based on core count
      // and give each thread its own seed)

      // pruning
      // - if we're too close to the hole with strokes remaining, wipe
      // - (if we're too close with no strokes remaining we might wipe too)
      // - if we're too far oob, wipe
      // - if we're smack on a hazard (TBA), wipe

      terrain = terrain_in;
      terrain_size = terrain_size_in;
      engine.seed(this->seed);

      // pick some tee location
      tee_point = GenerateTeeLocation(terrain, terrain_size);

      // we receive yardage from on high :D
      auto course = GenerateFromTee();
      return course;
    }

    impl::PositionTracker BruteForceCourseGenerator::GenerateFromTee() {
      // calculate shots to green
      int shots_to_green = 1;
      std::uniform_real_distribution<float>par_nudge(-10.0f, 10.0f);
      float calculation_yardage = yardage + par_nudge(engine);

      if (calculation_yardage > 225) {
        shots_to_green++;
      }

      if (calculation_yardage > 485) {
        shots_to_green++;
      }

      // to reuse point code:
      // generate a dummy "previous point" away from our center
      glm::vec2 dummy_prev = tee_point - (terrain_size / 2.0f);
      impl::PositionTracker seed_final;
      float res = GenerateFromPoint(
        tee_point,
        dummy_prev,
        shots_to_green,
        7,
        seed_final);

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
      if (shots_remaining <= 0) {
        // return 1 if we're pretty much "there" else 0
        return (distance_remaining < 0.5f ? 1.0f : 0.0f);
      }
      // nvm im just going to store the points >:)
      // single threaded, we use the seed anyway so just pass it in again idgnf
      // if final shot: let's bounce the logic to something else
      // (or: what if we specify a starting point, a tee point, and a yardage and let generation handle it)
      // (and then on the last shot we'll call a special method which targets a particular dist)

      // fuck it, let's just be greedy
      // keep the distro of points random
      // if the first point we test passes, use it
      // if we pass sample depth, return negative (to indicate that generation from this point failed)
      
      // some invalid conditions at which we can terminate right away
      if ((shots_remaining <= 1 && distance_remaining > 255) || (shots_remaining > 1 && distance_remaining <= 5)) {
        return 0.0f;
      }

      int sample_count = (1 << sample_depth) - 1;
      for (int i = 0; i < sample_count; i++) {
        // square sample, to prefer down the middle
        double theta = simple_dist(engine);
        theta *= theta;
        // flip along main axis
        theta *= (binary_dist(engine) == 1 ? 1 : -1) * M_PI_2;
        glm::vec2 main_axis = glm::normalize(current_point - previous_point);
        glm::vec2 cross_axis = glm::vec2(main_axis.y, -main_axis.x);
        // figure out which direction our shot is going in
        glm::vec2 travel_direction = glm::normalize(main_axis * static_cast<float>(cos(theta)) + cross_axis * static_cast<float>(sin(theta)));
        float shot_distance;
        if (shots_remaining > 1 && distance_remaining > 250.0f) {
          // take a shot!
          shot_distance = simple_dist(engine) * 35.0f + 220.0f;
        } else {
          shot_distance = impl::GetDistanceToCircle(current_point, travel_direction, tee_point, yardage);
          if (shot_distance > 265.0f) {
            // too long, see if we have better luck along a different path
            continue;
          }
        }

        glm::vec2 next_point = current_point + travel_direction * shot_distance;
        // we've found a candidate point
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
        double theta = radius_dist(engine);
        double r = angle_dist(engine);

        // normally we'd prune here or something
        glm::vec2 center(terrain_size.x / 2, terrain_size.y / 2);
        float max_radius = std::min(center.x, center.y);
        r = sqrt(r);
        
        return glm::vec2(center.x + cos(theta) * r * max_radius, center.y + sin(theta) * r * max_radius);
      }
    }
  }
}