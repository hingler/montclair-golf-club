#include "course/path/CompoundCurve.hpp"

#include <algorithm>

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

    void CompoundCurve::Translate(const glm::vec2& offset) {
      for (auto& segment : segments) {
        segment->Translate(offset);
      }
    }

    Rect CompoundCurve::GetBoundingBox() const {
      Rect result;

      if (segments.size() <= 0) {
        result.start = glm::vec2(0, 0);
        result.end = glm::vec2(0, 0);
      } else {
        result = segments[0]->GetBoundingBox();
        for (auto segment : segments) {
          Rect bounding_box_segment = segment->GetBoundingBox();
          result.start.x  = std::min(result.start.x,  bounding_box_segment.start.x);
          result.start.y  = std::min(result.start.y,  bounding_box_segment.start.y);
          result.end.x    = std::max(result.end.x,    bounding_box_segment.end.x);
          result.end.y    = std::max(result.end.y,    bounding_box_segment.end.y);
        }
      }

      return result;
    }

    void CompoundCurve::AddSegment(std::shared_ptr<BezierCurve> segment) {
      segments.push_back(segment);
    }

    double CompoundCurve::GetTimeForEndOfSpecifiedSegment(int segment) const {
      if (segment < 0) { return 0.0; }
      if (segment >= (segments.size() - 1)) { return 1.0; }
      float length = Length();
      float length_acc = 0.0;

      for (int i = 0; i <= segment; i++) {
        length_acc += segments[i]->Length();
      }

      return length_acc / length;
      
    }
  }
}