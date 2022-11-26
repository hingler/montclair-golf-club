#include "course/path/CompoundCurve.hpp"

namespace course {
  namespace path {
    CompoundCurve::CompoundCurve() {}

    glm::vec2 CompoundCurve::Sample(double time) const {
      if (segments.size() <= 0) {
        return glm::vec2(0, 0);
      }

      if (time <= 0.0) {
        return segments[0]->Sample(0.0);
      } else if (time >= 1.0) {
        return segments[segments.size() - 1] -> Sample(1.0);
      }

      double length = Length();

      double target_len = length * time;
      double prev_length_sum = 0.0;
      double length_sum = 0.0;
      for (int i = 0; i < segments.size(); i++) {
        double local_length = segments[i]->Length();
        length_sum += local_length;
        if (length_sum > target_len) {
          double local_time = (target_len - prev_length_sum) / local_length;
          return segments[i]->Sample(local_time);
        }

        prev_length_sum = length_sum;
      }

      return segments[segments.size() - 1]->Sample(1.0);
    }

    double CompoundCurve::Length() const {
      double length = 0.0;
      for (auto& segment : segments) {
        length += segment->Length();
      }

      return length;
    }

    void CompoundCurve::AddSegment(std::shared_ptr<BezierCurve> segment) {
      segments.push_back(segment);
    }
  }
}