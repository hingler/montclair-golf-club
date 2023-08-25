#ifndef GAUSSIAN_METABALL_H_
#define GAUSSIAN_METABALL_H_

#include <glm/glm.hpp>

namespace course {
  namespace sampler {
    namespace impl {
      /**
       * @brief Alternative metaball approach which makes use of a gaussian function
       * 
       */
      struct GaussianMetaball {
        // stdev distance
        double sigma;

        // size multiplier applied to underlying gaussian
        double intensity;

        // origin of this metaball
        glm::dvec2 origin;

      };
    }
  }
}

#endif // GAUSSIAN_METABALL_H_