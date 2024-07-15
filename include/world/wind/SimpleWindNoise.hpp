#ifndef MGC_SIMPLE_WIND_NOISE_H_
#define MGC_SIMPLE_WIND_NOISE_H_

// curl magnitude is affected by rate of change
// ie: the slope of our rate of change
// harsh winds require larger rates of change

// idea right now
// - perlin noise
// - v low frequency, VERY high amplitude
// - add some larger circular perturbations on top

#include "seed/direct/impl/SimplexHeightFunc.hpp"
namespace mgc {
  namespace world {
    class SimpleWindNoise {
     public:
      // p sure this is right
      SimpleWindNoise() : noise(3, 4096.0, 16384.0) {}
      double Sample(double x, double y) const {
        return noise.Sample(x, y);
      }

     private:
      _impl::SimplexHeightFunc noise;
    };
  }
}

#endif // MGC_SIMPLE_WIND_NOISE_H_
