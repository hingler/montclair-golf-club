#include "course/sampler/GaussianMetaballSampler.hpp"

#include "util/Gaussian.hpp"

#include "noise/simplex_noise.hpp"

#define ONE_SQRT_TWO_PI 0.39894228040143267793994605993438186847585863116493 // 1 / sqrt(2 * pi)
#define ONE_HALF 0.5

namespace course {
  namespace sampler {
    using namespace impl;

    double GaussianMetaballSampler::Sample(double x, double y) const {
      double accumulator = 0.0;
      for (auto& metaball : metaballs_) {
        double x_dist = x - metaball.origin.x;
        double y_dist = y - metaball.origin.y;
        double dist_squared = x_dist * x_dist + y_dist * y_dist;
        accumulator += util::Gaussian(dist_squared, metaball.sigma) * metaball.intensity;
      }

      return accumulator;
    }

    glm::vec2 GaussianMetaballSampler::Gradient(double x, double y) const {
      glm::vec2 accumulator = glm::vec2(0.0);
      for (auto& metaball : metaballs_) {
        double x_dist = metaball.origin.x - x;
        double y_dist = metaball.origin.y - y;
        double dist_squared = x_dist * x_dist + y_dist * y_dist;
        double dist = sqrt(dist_squared);

        // derivative of gaussian, in direction of metaball
        // abs val - should always be positive, pointing towards peak
        accumulator += glm::normalize(glm::vec2(x_dist, y_dist)) 
          * static_cast<float>(util::Derivative(dist, metaball.sigma) * metaball.intensity);
      }

      // TODO: check for accuracy :3

      // gist:
      // - pick a spot on path
      // - step along gradient until we reach some threshold intensity (away from fairway)
      // - place negative weights in a cluster (sand dune)

      // tba: how do we differentiate between water and sand?
      // - "fill in" - just flood from a point into anything below a certain target threshold
      return accumulator;
    }



    void GaussianMetaballSampler::Add(double x, double y, double sigma, double intensity) {
      metaballs_.push_back({ sigma, intensity, glm::dvec2(x, y) });
    }

    void GaussianMetaballSampler::Merge(const GaussianMetaballSampler& other, double intensity_mod) {
      for (auto& metaball : other.metaballs_) {
        Add(metaball.origin.x, metaball.origin.y, metaball.sigma, metaball.intensity * intensity_mod);
      }
    }

    void GaussianMetaballSampler::Clear() {
      metaballs_.clear();
    }
  }
}