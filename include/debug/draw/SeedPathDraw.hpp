#ifndef SEED_PATH_DRAW_H_
#define SEED_PATH_DRAW_H_

#include "seed/path/SeedPath.hpp"

#include "util/LineUtil.hpp"

#include <memory>
#include <limits>

namespace mgc {
  class SeedPathDraw {
   public:
    SeedPathDraw(std::shared_ptr<SeedPath> path, double width) : path_(path), width_square_(width * width) {}
    double Sample(double x, double y) {
      glm::dvec2 sample(x, y);
      glm::dvec2 prev_point = path_->GetRoot()->get(0);
      double min_dist = std::numeric_limits<double>::max();
      auto itr = path_->begin();
      // skip first element
      ++itr;

      // oh shit! this won't cover path breaks
      // (is there a way that i can do that? alt: just leave it be for now)

      while (itr != path_->end()) {
        if (itr->jump) {
          prev_point = itr->point;
        } else {
          double dist;
          glm::dvec2 pt_dist = (prev_point - itr->point);

          // last step was a jump - continue

          if (util::length_squared(pt_dist) < 0.000001) {
            // points are overlapping - just find dist to whichever
            dist = util::length_squared(sample - prev_point);
          } else {
            dist = util::dist_to_line(sample, prev_point, itr->point);
          }

          // update prev point
          prev_point = itr->point;

          min_dist = glm::min(
            min_dist,
            dist
          );

        }

        ++itr;
      }

      // next thing to do
      // -

      // yippee!
      return (min_dist < width_square_ ? 1.0 : 0.0);
    }
   private:
    std::shared_ptr<SeedPath> path_;
    const double width_square_;
  };
}

#endif // SEED_PATH_DRAW_H_
