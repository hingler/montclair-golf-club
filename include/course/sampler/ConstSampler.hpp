#ifndef MGC_CONST_SAMPLER_H_
#define MGC_CONST_SAMPLER_H_

#include <cstddef>
#include <glm/glm.hpp>

namespace mgc {
  namespace sampler {
    struct ConstSampler {
      ConstSampler(double val) : val(val) {}

      double Sample(double x, double y) const {
        return val;
      }

      glm::vec4 Sample(double x, double y, size_t index) const {
        return glm::vec4(val);
      }

     private:
      const double val;
    };
  }
}

#endif // MGC_CONST_SAMPLER_H_
