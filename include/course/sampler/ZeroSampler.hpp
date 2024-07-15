#ifndef MGC_ZERO_SAMPLER_H_
#define MGC_ZERO_SAMPLER_H_

#include <cstddef>
#include <glm/glm.hpp>

namespace mgc {
  namespace sampler {
    struct ZeroSampler {
      double Sample(double x, double y) const {
        return 0.0;
      }

      glm::vec4 Sample(double x, double y, size_t index) const {
        return glm::vec4(0.0);
      }
    };
  }
}

#endif // MGC_ZERO_SAMPLER_H_
