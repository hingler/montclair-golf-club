#ifndef MGC_FEATURE_SPEC_H_
#define MGC_FEATURE_SPEC_H_

#include "glm/glm.hpp"
#include <algorithm>
#include <cstddef>

namespace mgc {
  namespace mgc_course {
    struct FeatureSpec {
      size_t runtime_id;
      glm::dvec2 position;
      double rotation_degrees;
      double scale;

      glm::dvec2 GetOrigin(const glm::dvec2& size) const {
        return position - size / 2.0;
      }

      bool operator==(const FeatureSpec& other) const {
        // might need to rope this in at some point
        return (
          runtime_id == other.runtime_id
        );
      }
    };
  }
}

namespace std {
  template <>
  struct std::hash<mgc::mgc_course::FeatureSpec> {
    std::hash<size_t> size_t_hash;
    size_t operator()(const mgc::mgc_course::FeatureSpec& other) const {
      return size_t_hash(other.runtime_id);
    }
  };
}

#endif // MGC_FEATURE_SPEC_H_
