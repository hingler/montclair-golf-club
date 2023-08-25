#ifndef GAUSSIAN_H_
#define GAUSSIAN_H_

namespace util {

  /**
   * @brief Calculates gaussian
   * 
   * @param x_squared sample point squared
   * @param sigma stdev
   * @return double result on gaussian curve
   */
  double Gaussian(double x_squared, double sigma);

  /**
   * @brief Calculates gaussian with falloff - mu assumed 0
   * 
   * @param x_squared sample point squared
   * @param sigma stdev
   * @param z_start_sigma z at which to begin falloff
   * @param z_end_sigma z at which to end falloff
   * @return double result on gaussian curve
   */
  double GaussianFalloff(double x_squared, double sigma, double z_start_sigma, double z_end_sigma);

  /**
   * @brief Calculates derivative of gaussian - mu assumed 0
   * 
   * @param x sample point
   * @param sigma stdev
   * @return double 
   */
  double Derivative(double x, double sigma);
}

#endif // GAUSSIAN_H_