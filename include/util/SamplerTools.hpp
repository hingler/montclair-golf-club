#ifndef SAMPLER_TOOLS_H_
#define SAMPLER_TOOLS_H_

#include "type/sampler_type.hpp"

#include <glm/glm.hpp>

#define _SAMPLER_STEP 0.005
#define _GRADIENT_FACTOR (0.5 / _SAMPLER_STEP)

namespace util {
  template <typename Sampler>
  glm::dvec2 Gradient(Sampler& sampler, double x, double y) {
    // calculates gradient of any sampler-spec
    static_assert(type::sampler_type<Sampler>::value);
    // gradient here is effectively just sobol filter, so let's do that
    double sample_l = sampler.Sample(x - _SAMPLER_STEP, y);
    double sample_r = sampler.Sample(x + _SAMPLER_STEP, y);
    double sample_u = sampler.Sample(x, y + _SAMPLER_STEP);
    double sample_d = sampler.Sample(x, y - _SAMPLER_STEP);

    return glm::dvec2((sample_r - sample_l) * _GRADIENT_FACTOR, (sample_u - sample_d) * _GRADIENT_FACTOR);
  }
}

#endif // SAMPLER_TOOLS_H_