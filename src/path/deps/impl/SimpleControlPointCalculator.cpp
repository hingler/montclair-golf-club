#include "path/deps/impl/SimpleControlPointCalculator.hpp"
#include <algorithm>
#include <glm/glm.hpp>

#define SHOT_MIN_THRESHOLD 190.0
#define SHOT_MAX_THRESHOLD 260.0

namespace mgc {
  namespace deps {
    namespace impl {

      std::vector<glm::dvec2> SimpleControlPointCalculator::GetControlPoints(
        const std::vector<glm::dvec2>& course_path,
        size_t seed
      ) const {
        // everything can be inferred from stops
        if (course_path.size() <= 0) {
          return std::vector<glm::dvec2>();
        }
        double len_sum = 0;
        for (size_t i = 1; i < course_path.size(); i++) {
          len_sum += glm::length(course_path.at(i) - course_path.at(i - 1));
        }

        size_t par = GetPar(len_sum);
        // bias spacing greedily
        // want to ensure that we don't go above, like, 240 yards to next hole
        std::vector<glm::dvec2> res;
        res.push_back(course_path.at(0));

        // dist thus far
        double len_acc = 0.0;
        // dist since last stop
        double len_cur = 0.0;
        // set a min, set a max
        size_t index = 1;
        // shot count
        engine.seed(seed);
        for (size_t s = 1; s < (par - 2); s++) {
          // set arb len threshold
          len_cur = 0.0;
          double len_remaining = (len_sum - len_acc);
          // assume max shot dist
          double min_shot_len = len_remaining - (SHOT_MAX_THRESHOLD * (s - (par - 2)));

          double len_min = std::max(SHOT_MIN_THRESHOLD, min_shot_len);
          double len_max = std::min(SHOT_MAX_THRESHOLD, len_remaining);

          // double check
          len_min = std::min(len_min, len_max);

          std::uniform_real_distribution<double> yardage_dist(len_min, len_max);

          double len_target = yardage_dist(engine);
          while (index < course_path.size() && len_cur < len_target) {
            double inc = glm::length(course_path.at(index) - course_path.at(index - 1));
            len_acc += inc;
            len_cur += inc;

            index++;
          }

          res.push_back(course_path.at(index));
        }

        res.push_back(course_path.at(course_path.size() - 1));

        return res;
      }

      size_t SimpleControlPointCalculator::GetPar(double len_sum) const {
        return 3 + static_cast<int>(std::floor((len_sum + 30) / 235));
      }
    }
  }
}
