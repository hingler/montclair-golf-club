#include "sdf/type/fairway/TeeboxSDF.hpp"
#include "glm/trigonometric.hpp"

#include <gog43/Logger.hpp>

namespace mgc {
  namespace fairway {
    TeeboxSDF::TeeboxSDF(
      const glm::dvec2& center,
      const glm::dvec2& direction,
      const glm::dvec2& dimensions
    ) : delegate(center, dimensions * 2.0, glm::degrees(glm::atan(-direction.x, direction.y))) {}

    double TeeboxSDF::Sample(double x, double y) const {
      return delegate.Sample(x, y);
    }
  }
}
