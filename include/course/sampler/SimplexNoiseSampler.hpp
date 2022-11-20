#ifndef SIMPLEX_NOISE_SAMPLER_H_
#define SIMPLEX_NOISE_SAMPLER_H_

#include <glm/glm.hpp>

#include "course/sampler/ISampler.hpp"

namespace course {
  namespace sampler {
    class SimplexNoiseSampler : public ISampler<float> {
    public:
      SimplexNoiseSampler(const glm::vec3& scale, int octaves);
      float Sample(float x, float y) const override;

      double time = 0.0;
    private:
      // noise scale (xyz)
      glm::vec3 scale_;

      // number of octaves of noise to stack
      // octaves are offset and rotated from one another
      int octaves_;
    };
  }
}

#endif // SIMPLEX_NOISE_SAMPLER_H_