#ifndef SIMPLEX_HEIGHT_FUNC_H_
#define SIMPLEX_HEIGHT_FUNC_H_

#include <glm/gtc/noise.hpp>

namespace mgc {
  namespace _impl {
    struct SimplexHeightFunc {
      // modify lacunarity?
      // modify decay?
      // later lole
      SimplexHeightFunc(
        size_t octaves,
        double amplitude,
        double scale
      ): octaves(octaves), amplitude(amplitude), scale(scale) {}

      // https://thebookofshaders.com/13/
      // https://www.redblobgames.com/maps/terrain-from-noise/#octaves
      double Sample(double x, double y) const {
        glm::dvec2 pos(x, y);
        // apply scale first
        pos /= scale;
        double acc = 0.0;

        double weight = 0.5;

        for (size_t i = 0; i < octaves; i++) {
          acc += glm::simplex(pos) * weight;
          pos *= 2.0;
          weight *= 0.5;
        }

        return acc * amplitude;
      }
     private:
      const size_t octaves;
      const double amplitude;
      const double scale;
    };
  }
}

#endif // SIMPLEX_HEIGHT_FUNC_H_

// kinda stuck now - what do i want to test?
// - plug in a height func (prob some simplex noise)
// - trace it out and make sure that we follow it
//   - (ie: demo app :3)
