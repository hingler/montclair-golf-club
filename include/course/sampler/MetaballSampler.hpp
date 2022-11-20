#ifndef METABALL_SAMPLER_H_
#define METABALL_SAMPLER_H_

#include "course/sampler/ISampler.hpp"
#include "course/sampler/impl/Metaball.hpp"

#include <vector>

namespace course {
  namespace sampler {
    /**
     * @brief Sampler which uses metaballs to represent swatches of terrain
     */
    class MetaballSampler : public ISampler<float> {
    public:
      float Sample(float x, float y) const override;

      /**
       * @brief Creates a new metaball in this sampler.
       * 
       * @param x - x coordinate.
       * @param y - y coordinate.
       * @param radius_mul - radius of this metaball.
       */
      void AddMetaball(float x, float y, float radius_mul);

      // todo: metaball edges are jagged, add a softening power? (ie do some funky math to smooth out?)

      /**
       * @brief threshold for metaball fill. behavior undefined if <= 0.
       */
      float threshold = 1.0f;
    private:
      // metaball storage
      std::vector<impl::Metaball> metaballs_;
    };
  }
}

#endif // METABALL_SAMPLER_H_