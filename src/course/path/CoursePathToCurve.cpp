#include "course/path/CoursePathToCurve.hpp"

#define SPLINE_TENSION_FACTOR (1.0 / 3.0)

namespace course {
  namespace path {
    static glm::vec2 GetHandleVector_(const std::vector<glm::vec2>& points, int index, float tension);


    CompoundCurve CoursePathToCurve(const CoursePath& path, float tension) {
      CompoundCurve res;
      auto& data = path.course_path;
      for (int i = 0; i < (data.size() - 1); i++) {
        // grab cur point and next point
        glm::vec2 p0 = data[i];
        glm::vec2 p3 = data[i + 1];
        glm::vec2 p1 = p0 + GetHandleVector_(data, i, tension);
        glm::vec2 p2 = p1 - GetHandleVector_(data, i + 1, tension);
        std::shared_ptr<BezierCurve> curve = std::make_shared<BezierCurve>(p0, p1, p2, p3);
        res.AddSegment(curve);
      }

      return res;
    }

    // fetches handle facing next point, if present
    // if not: flips handle of nearest available point
    static glm::vec2 GetHandleVector_(const std::vector<glm::vec2>& points, int index, float tension) {
      if (index < 0 || index >= points.size()) {
        return glm::vec2(0);
      }

      if (points.size() <= 2) {
        return points[index];
      }

      glm::vec2 handle_temp;
      if (index == 0 || index == points.size() - 1) {
        glm::vec2 neighbor;
        if (index == 0) {
          // flip to get back facing
          handle_temp = -GetHandleVector_(points, 1, tension);
          neighbor = points[1];
        } else {
          // do not flip handle, we want to reflect forward facing handle
          handle_temp = GetHandleVector_(points, points.size() - 2, tension);
          neighbor = points[points.size() - 2];
        }

        // line dir
        glm::vec2 main_axis = glm::normalize(handle_temp - neighbor);
        // reflect neighbor's facing handle to get our handle
        handle_temp = glm::reflect(handle_temp, main_axis);

        // 0 handle is already forward facing
        if (index == points.size() - 1) {
          // flip handle, so forward facing
          handle_temp = -handle_temp;
        }

        return handle_temp;
      }

      glm::vec2 tangent_vec = points[index + 1] - points[index - 1];
      tangent_vec *= (tension * SPLINE_TENSION_FACTOR);
      return tangent_vec;
    }
  }
}