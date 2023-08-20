#include "course/sampler/grass/TeeSampler.hpp"


namespace course {
  using namespace path;
  namespace sampler {
    namespace grass {
      TeeSampler::TeeSampler(const CoursePath& course_path) {
        if (course_path.course_path.size() > 0) {
          auto& path = course_path.course_path;
          center = path[0];
          direction_facing = glm::normalize(path[1] - path[0]);
          direction_cross = glm::vec2(direction_facing.y, -direction_facing.x);
        }
      }

      float TeeSampler::Sample(float x, float y) const {
        glm::vec2 relative_point(center.x - x, center.y - y);
        float main_axis_dist = abs(glm::dot(relative_point, direction_facing)) * 2;
        float cross_axis_dist = abs(glm::dot(relative_point, direction_cross)) * 2;

        // tba: corner rounding? smoothing?
        if (main_axis_dist < length && cross_axis_dist < width) {
          return 1.0f;
        }
        
        return 0.0f;
      }
    }
  }
}