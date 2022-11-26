#include "course/path/BezierCurve.hpp"

namespace course {
  namespace path {
    BezierCurve::BezierCurve(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
      this->p0 = p0;
      this->p1 = p1;
      this->p2 = p2;
      this->p3 = p3;

      // get a length estimate
      float len_est = glm::length(p1 - p0) + glm::length(p2 - p1) + glm::length(p3 - p2);

      int samples_count = (std::max(static_cast<int>(ceil(len_est / 100)), 1) * 100);
      ArcLengthParameterize_(samples_count);
    }

    glm::vec2 BezierCurve::Sample(double time) const {
      double time_absolute = GetParameterizedTime_(time);
      return SampleBezier_(time_absolute);
    }

    double BezierCurve::Length() const {
      if (distance_points.size() > 0) {
        return distance_points[distance_points.size() - 1];
      }

      return 0.0;
    }

    glm::vec2 BezierCurve::SampleBezier_(double t) const {
      float t0 = (1 - t);

      return p0 *        static_cast<float>(t0 * t0 * t0) +
             p1 * static_cast<float>(3.0f * t0 * t0 *  t) +
             p2 * static_cast<float>(3.0f * t0 *  t *  t) +
             p3 *         static_cast<float>(t *  t *  t);
    }

    void BezierCurve::ArcLengthParameterize_(int num_samples) {
      distance_points.clear();
      double time_step = 1.0 / num_samples;
      double time = 0.0;
      double dist = 0.0;

      distance_points.reserve(num_samples + 1);

      glm::vec2 current_point;
      glm::vec2 last_point = glm::vec2(0.0);

      last_point = p0;

      distance_points.push_back(0.0);

      for (int i = 0; i < num_samples; i++) {
        time += time_step;
        current_point = SampleBezier_(time);
        dist += glm::length(current_point - last_point);
        distance_points.push_back(dist);
        last_point = current_point;
      }
    }

    double BezierCurve::GetParameterizedTime_(double t_absolute) const {
      if (t_absolute <= 0.0) {
        return 0.0;
      } else if (t_absolute >= 1.0) {
        return 1.0;
      }

      size_t size = distance_points.size();
      if (size <= 0) {
        return 0.0;
      }
      
      double desired_len = t_absolute * distance_points[size - 1];
      double current_dist = 0;
      double last_dist = 0;
      size_t sample_point = 1;

      do {
        last_dist = current_dist;
        current_dist = distance_points[sample_point];
        if (current_dist > desired_len) {
          break;
        }
      } while (++sample_point < size);

      if (abs(current_dist - last_dist) < 0.000001) {
        return 0.0;
      }

      double time_ceil = static_cast<double>(sample_point) / size;
      double time_floor = static_cast<double>(sample_point - 1) / size;
      double mix_param = (desired_len - last_dist) / (current_dist - last_dist);
      double res = time_floor + (time_ceil - time_floor) * mix_param;
      return res;
    }
  }
}