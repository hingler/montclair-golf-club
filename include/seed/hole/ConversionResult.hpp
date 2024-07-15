#ifndef MGC_CONVERSION_RESULT_H_
#define MGC_CONVERSION_RESULT_H_

#include "corrugate/FeatureBox.hpp"
#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "path/CourseBundle.hpp"

#include <memory>

namespace mgc {
  struct ConversionResult {
    std::unique_ptr<cg::BaseSmoothingSamplerBox> box;
    CourseBundle data;
  };

  struct HoleInfo {
    std::shared_ptr<const cg::BaseSmoothingSamplerBox> box;
    CourseBundle data;

    glm::dvec2 GetTeePosition() const {
      return box->GetOrigin() + data.course_path[0];
    }

    glm::dvec2 GetHolePosition() const {
      return box->GetOrigin() + data.hole;
    }

    cg::FeatureBox GetHoleBoundingBox() const {
      glm::dvec2 start = data.GetBoundingBoxStart();
      glm::dvec2 end = data.GetBoundingBoxEnd();

      return cg::FeatureBox(start, end - start);
    }

    glm::dvec2 GetTeeDirection() const {
      return glm::normalize(data.tee_direction);
    }

    // XY: direction in 2D; Z: shot distance
    glm::dvec3 GetBestShotDirection(
      const glm::dvec2& ball_global,
      double max_yardage
    ) const {
      glm::dvec2 ball_local = ball_global - box->GetOrigin();
      size_t index_near = data.GetNearestIndex(ball_local);



      double max_dist = 0.0;

      glm::dvec2 best_point = data.hole;
      if (glm::length(best_point - ball_local) > max_yardage) {
        // need to find another point
        for (int i = index_near; i < data.course_path.size(); i++) {
          glm::dvec2 target_local = data.course_path.at(i);
          double dist = glm::length(target_local - ball_local);
          // need to also estimate if the target point is safe
          if (dist < max_yardage && dist > max_dist) {
            max_dist = dist;
            best_point = data.course_path.at(i);
          }
        }
      }

      glm::dvec2 direction = glm::normalize(best_point - ball_local);
      double yardage = glm::length(best_point - ball_local);

      return glm::dvec3(
        direction.x,
        direction.y,
        yardage
      );
    }

    bool operator==(const HoleInfo& other) const {
      return box == other.box;
    }

    bool operator!=(const HoleInfo& other) const {
      return (box != other.box);
    }
  };
}

namespace std {
  template <>
  struct hash<mgc::HoleInfo> {
    std::hash<std::shared_ptr<const cg::BaseSmoothingSamplerBox>> ptr_hash;
    size_t operator()(const mgc::HoleInfo& other) const {
      return ptr_hash(other.box);
    }
  };
}

#endif // MGC_CONVERSION_RESULT_H_
