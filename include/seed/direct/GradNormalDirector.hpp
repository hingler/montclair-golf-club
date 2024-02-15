#ifndef GRAD_NORMAL_DIRECTOR_H_
#define GRAD_NORMAL_DIRECTOR_H_

#include "seed/direct/DirectorBase.hpp"

#include <memory>

namespace mgc {
  template <typename HeightMap>
  class GradNormalDirector : public DirectorBase {
   public:
    GradNormalDirector(const std::shared_ptr<HeightMap> height, double weight) : DirectorBase(weight), height(height) {

    }
   private:
    glm::dvec2 SampleDirection_impl(const DirectorInfo& context) const override {
      glm::dvec2 dx = context.point + glm::dvec2(GRAD_EPSILON, 0.0);
      glm::dvec2 dy = context.point + glm::dvec2(0.0, GRAD_EPSILON);

      double sample = height->Sample(context.point.x, context.point.y);

      double grad_dx = (height->Sample(dx.x, dx.y) - sample) / GRAD_EPSILON;
      double grad_dy = (height->Sample(dy.x, dy.y) - sample) / GRAD_EPSILON;

      // handle rotation here (oh yeah fuck it - this is just a curl right? yes - no change in z, so yes!)
      glm::dvec2 grad_normal(-grad_dy, grad_dx);

      // return gradient, normalized in this case
      if ((grad_normal.x * grad_normal.x + grad_normal.y * grad_normal.y) > 1.0) {
        grad_normal = glm::normalize(grad_normal);
      }

      return grad_normal;
    }

    static constexpr double GRAD_EPSILON = 0.00001;
    std::shared_ptr<HeightMap> height;
  };
}

#endif // GRAD_NORMAL_DIRECTOR_H_