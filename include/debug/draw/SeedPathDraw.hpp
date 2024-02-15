#ifndef SEED_PATH_DRAW_H_
#define SEED_PATH_DRAW_H_

#include "seed/path/SeedPath.hpp"

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

          if (length_squared(pt_dist) < 0.000001) {
            // points are overlapping - just find dist to whichever
            dist = length_squared(sample - prev_point);
          } else {
            dist = dist_to_line(sample, prev_point, itr->point);
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
    static inline double length_squared(const glm::dvec2& line) {
      return line.x * line.x + line.y * line.y;
    }

    double dist_to_line(const glm::dvec2& sample, const glm::dvec2& start, const glm::dvec2& end) {
      // project dist to point onto line, subtract projected component
      // if projection is not on the line, then use dist to point
      glm::dvec2 e_dist = end - sample;

      glm::dvec2 line_proj = glm::normalize(end - start);

      // dot gives us mag of tangent component
      // subtract line to get point
      glm::dvec2 perp_point = end - (glm::dot(e_dist, line_proj) * line_proj);

      glm::dvec2 bb_min(glm::min(start.x, end.x), glm::min(start.y, end.y));
      glm::dvec2 bb_max(glm::max(start.x, end.x), glm::max(start.y, end.y));

      // perp point is outside bounding box ie not on line - use line dist
      if (
          perp_point.x < bb_min.x
      ||  perp_point.y < bb_min.y
      ||  perp_point.x > bb_max.x
      ||  perp_point.y > bb_max.y
      ) {
        glm::dvec2 s_dist = start - sample;

        return glm::min(
          length_squared(s_dist), 
          length_squared(e_dist)
        );
      } else {
        return length_squared(sample - perp_point);
      }
      
    }
    std::shared_ptr<SeedPath> path_;
    const double width_square_;
  };
}

#endif // SEED_PATH_DRAW_H_