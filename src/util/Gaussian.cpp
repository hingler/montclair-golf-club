#include "util/Gaussian.hpp"

#include <glm/glm.hpp>

#define ONE_SQRT_TWO_PI 0.39894228040143267793994605993438186847585863116493 // 1 / sqrt(2 * pi)

namespace util {
  double Gaussian(double x_squared, double sigma) {
    double sigma_squared = sigma * sigma;
    return ((1.0 / sigma) * ONE_SQRT_TWO_PI) * exp(-0.5 * (x_squared / sigma_squared));
  }

  double GaussianFalloff(double x_squared, double sigma, double fo_start_z, double fo_end_z) {
    double res = Gaussian(x_squared, sigma);
    double z_score = x_squared / (sigma * sigma);
    return glm::clamp((1.0 - glm::smoothstep(fo_start_z, fo_end_z, z_score)) * res, 0.0, 1.0);
  }

  double Derivative(double x, double sigma) {
    double x_squared = x * x;
    double sigma_squared = sigma * sigma;

    return Gaussian(x_squared, sigma) * (x / sigma_squared);
  }
}