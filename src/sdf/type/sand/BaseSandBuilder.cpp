#include "sdf/type/sand/BaseSandBuilder.hpp"
#include "sdf/Bundle.hpp"
#include <cmath>
#include <random>

// power applied to sand placement along cross axis
#define DIST_POW 0.03125

// displacement along cross axis, in yards
#define DIST_MUL 55.0

// pow applied to placement along main axis
#define WIGGLE_POW 0.25

// number of yards to wiggle by, along main axis
#define WIGGLE_MUL 56.0

// min size of sand circles
#define SAND_MIN 9.5

namespace mgc {
  namespace sand {
    std::shared_ptr<BaseSandSDF> BaseSandBuilder::Get(
      const CourseBundle& bundle,
      std::mt19937_64& engine
    ) const {
      // pick a few "candidate" points around path stops
      // scatter out - typ avoid fairway center
      // - 1/-1 for l/r sign + v low power to push out
      // once we pick a point: i think the goal should be to aim for a "rough shape" vs a blob
      // (tba ig)

      auto res = std::make_shared<CPPBundle>(23.5);

      // for placing sand
      // placement along cross axis
      std::uniform_int_distribution<int> dist_sign(0, 1);
      std::uniform_real_distribution<double> dist_mag(0.0, 1.0);

      // wiggle applied to the control point we sample from
      // (would like to do)
      // placement along main axis
      std::uniform_int_distribution<int> wiggle_sign(0, 1);
      std::uniform_real_distribution<double> wiggle_mag(0.0, 1.0);

      std::uniform_int_distribution<int> pit_count(0, 3);
      std::uniform_int_distribution<int> pit_circs(2, 4);

      std::uniform_real_distribution<double> sand_dist(-20.0, 20.0);
      std::normal_distribution<double> sand_rad(12.0, 6.0);

      for (size_t p = 1; p < bundle.stop_indices.size(); p++) {
        int pits = pit_count(engine);
        for (size_t s = 0; s < pits; s++) {
          double cross_offset = (dist_sign(engine) * 2 - 1) * std::pow(dist_mag(engine), DIST_POW) * DIST_MUL;
          double main_offset = (wiggle_sign(engine) * 2 - 1) * std::pow(wiggle_mag(engine), WIGGLE_POW) * WIGGLE_MUL;

          double pit_yardage = bundle.IndexToYardage(bundle.stop_indices.at(p)) + main_offset;
          double pit_index = bundle.IndexAtDist(pit_yardage);

          glm::dvec2 sand_center = bundle.SampleAtDist(pit_yardage) + (bundle.GetNormal(pit_index) * cross_offset);

          int circles = pit_circs(engine);
          for (size_t i = 0; i < circles; i++) {
            glm::dvec2 circ_point = sand_center + glm::dvec2(sand_dist(engine), sand_dist(engine));
            res->AddCircle(circ_point.x, circ_point.y, std::max(sand_rad(engine), SAND_MIN));
          }
        }
      }

      return std::make_shared<BaseSandSDF>(res);
    }
  }
}
