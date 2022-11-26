#include "course/sampler/MetaballSampler.hpp"

#include <cmath>

namespace course {
  namespace sampler {
    float MetaballSampler::Sample(float x, float y) const {
      float accolator = 0.0f;
      float dist_x;
      float dist_y;
      for (const impl::Metaball& metaball : metaballs_) {
        dist_x = x - metaball.position_x;
        dist_y = y - metaball.position_y;
        accolator += (1.0f / (sqrt(dist_x * dist_x + dist_y * dist_y) * metaball.radius_factor));
      }

      return (accolator > threshold ? 1.0f : 0.0f);
    }

    void MetaballSampler::AddMetaball(float x, float y, float radius_mul) {
      metaballs_.push_back({ x, y, 1.0f / radius_mul });
    }
  }
}