#include "image/filter/GaussianBlur.hpp"

#include <cmath>

namespace image {
  namespace filter {
    double GetGaussianIntensity(double sigma, double dist) {
      float e_coeff = exp(-((dist * dist) / (2.0 * sigma * sigma)));
      float fract = 1.0 / sqrt(2 * M_PI * sigma * sigma);
      return e_coeff * fract;
    }
  }
}