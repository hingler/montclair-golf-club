#ifndef GRADIENT_DRAW_H_
#define GRADIENT_DRAW_H_

#include <memory>

#include <glm/glm.hpp>

namespace mgc {
  template <typename HeightMap>
  class GradientDraw {
   public:
    GradientDraw(std::shared_ptr<HeightMap> map, double offset, double step_size) : map(map), offset(offset), step_size(step_size) {}
    double Sample(double x, double y) {
      double height_sample = map->Sample(x, y) + offset;
      // fract it!
      return glm::fract(height_sample / step_size);
    }
   private:
    std::shared_ptr<HeightMap> map;
    double offset;
    double step_size;
  };
}

#endif // GRADIENT_DRAW_H_