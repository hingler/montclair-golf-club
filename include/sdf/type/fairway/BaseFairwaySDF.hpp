#ifndef BASE_FAIRWAY_SDF_H_
#define BASE_FAIRWAY_SDF_H_

#include "sdf/CPPBundle.hpp"
#include "sdf/type/fairway/TeeboxSDF.hpp"

#include <memory>

namespace mgc {
  namespace fairway {
    // sdf for base fairway - incorporates a teebox and a bundle
    class BaseFairwaySDF {
     public:
      BaseFairwaySDF(
        const std::shared_ptr<CPPBundle>& bundle,
        const glm::dvec2& center,
        const glm::dvec2& direction,
        const glm::dvec2& dimensions
      );

      double Sample(double x, double y) const;
      std::shared_ptr<CPPBundle> GetBundle();
      std::shared_ptr<const TeeboxSDF> GetTeebox();
     private:
      std::shared_ptr<CPPBundle> bundle;
      std::shared_ptr<const TeeboxSDF> teebox;
    };
  }
}

#endif  // BASE_FAIRWAY_SDF_H_
