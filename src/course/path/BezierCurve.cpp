#include "course/path/BezierCurve.hpp"

#include <algorithm>
#include <iostream>

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

    void BezierCurve::Translate(const glm::vec2& offset) {
      // distances remain the same
      p0 += offset;
      p1 += offset;
      p2 += offset;
      p3 += offset;
    }

    Rect BezierCurve::GetBoundingBox() const {
      glm::vec2 extrema_x = GetExtrema(0);
      glm::vec2 extrema_y = GetExtrema(1);

      glm::vec2 x_min_sample = Sample(extrema_x.x);
      glm::vec2 x_max_sample = Sample(extrema_x.y);
      glm::vec2 y_min_sample = Sample(extrema_y.x);
      glm::vec2 y_max_sample = Sample(extrema_y.y);

      std::cout << "curve bb: " << x_min_sample.x << ", " << y_min_sample.y << std::endl;

      Rect result;
      result.start = glm::vec2(x_min_sample.x, y_min_sample.y);
      result.end = glm::vec2(x_max_sample.x, y_max_sample.y);

      // tba: write trivial tests for this
      return result;
    }

    glm::vec2 BezierCurve::SampleBezier_(double t) const {
      float t0 = (1 - t);

      return p0 *        static_cast<float>(t0 * t0 * t0) +
             p1 * static_cast<float>(3.0f * t0 * t0 *  t) +
             p2 * static_cast<float>(3.0f * t0 *  t *  t) +
             p3 *         static_cast<float>(t *  t *  t);
    }

    // (3D - 9C + 9B - 3A) * t^2 + (6C - 12B + 6A) * t + (3B - 3A) * t
    // four target points: endpoints, and derivatives wrt t
    
    // returns t min max along axis
    glm::vec2 BezierCurve::GetExtrema(int axis) const {
      // https://stackoverflow.com/questions/24809978/calculating-the-bounding-box-of-cubic-bezier-curve
      if (axis != 0 && axis != 1) {
        return glm::vec2(-1.0f, -1.0f);
      }

      // point coords
      float pa = p0[axis];
      float pb = p1[axis];
      float pc = p2[axis];
      float pd = p3[axis];

      float a = 3 * pd - 9 * pc + 9 * pb - 3 * pa;  // t^2
      float b = 6 * pc - 12 * pb + 6 * pa;          // t
      float c = 3 * pb - 3 * pa;                    // const fac

      // positive soln
      // accruing solutions
      float discriminant = b * b - 4 * a * c;
      float end_min = 0.0;
      float end_max = 1.0;
      float quad_neg = end_min;
      float quad_pos = end_max;

      if (discriminant > 0 && a != 0) {
        quad_neg = (-b - sqrt(discriminant)) / (2 * a);
        quad_pos = (-b + sqrt(discriminant)) / (2 * a);
      } else if (discriminant == 0 && a != 0) {
        // ignore sqrt
        quad_neg = -b / (2 * a);
        quad_pos = quad_neg;
      } // else: disc is imaginary, or a = 0 - in either case, endpoints only

      glm::vec2 result;
      // can't have off
      quad_neg = std::clamp(quad_neg, 0.0f, 1.0f);
      quad_pos = std::clamp(quad_pos, 0.0f, 1.0f);
      result.x = std::min(std::min(end_min, end_max), std::min(quad_neg, quad_pos));
      result.y = std::max(std::max(end_min, end_max), std::max(quad_neg, quad_pos));

      return result;
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