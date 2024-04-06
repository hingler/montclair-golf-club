#ifndef INVERSE_SDF_H_
#define INVERSE_SDF_H_

#include <glm/glm.hpp>

#include <memory>

namespace mgc {
  template <typename SDFType>
  class InverseSDF {
   public:
    InverseSDF(const std::shared_ptr<SDFType>& sdf) : sdf(sdf) {}

    double Sample(double x, double y) {
      return -(sdf->Sample(x, y));
    }

    double Sample(const glm::dvec2& point) {
      return -(sdf->Sample(point.x, point.y));
    }

    double Sample(double x, double y, double z) {
      return -(sdf->Sample(x, y, z));
    }

   private:
    std::shared_ptr<SDFType> sdf;
  };
}

#endif // INVERSE_SDF_H_
