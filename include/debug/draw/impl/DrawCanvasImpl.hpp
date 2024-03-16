#ifndef DRAW_CANVAS_IMPL_H_
#define DRAW_CANVAS_IMPL_H_

#include "debug/draw/impl/DrawCanvas.hpp"

#include <memory>

namespace mgc {
  namespace impl {
    template <typename SamplerType> class DrawCanvasImpl : public DrawCanvas {
     public:
      DrawCanvasImpl(std::shared_ptr<SamplerType> sampler, const glm::dvec4& color) : sampler_(sampler), color_(color) {}
      glm::dvec4 Sample(double x, double y) const override {
        double sample = sampler_->Sample(x, y);
        sample = glm::clamp(sample, 0.0, 1.0);

        return color_ * sample;
      }

     private:
      std::shared_ptr<SamplerType> sampler_;
      glm::dvec4 color_;
    };
  } // namespace impl
} // namespace mgc

#endif // DRAW_CANVAS_IMPL_H_
