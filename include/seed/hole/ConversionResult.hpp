#ifndef MGC_CONVERSION_RESULT_H_
#define MGC_CONVERSION_RESULT_H_

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
