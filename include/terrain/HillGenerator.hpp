#ifndef HILL_GENERATOR_H_
#define HILL_GENERATOR_H_

#include <glm/glm.hpp>

#include <random>
#include <unordered_map>

#include <glm/gtx/hash.hpp>

#include "noise/TurbulentDisplace.hpp"

namespace terrain {
  namespace _impl {
    // info stored when generating hills
    struct HillInfo {
      double scatter_x;
      double scatter_y;
      double intensity;
      bool  fill;
      double local_sigma;
    };
  }
  /**
   * @brief Generates hills which are sample-able
   */
  class HillGenerator {
   public:
   /**
    * @brief Samples this generator.
    * 
    * @param x - x coord of sample
    * @param y - y coord of sample 
    * @return double 
    */
    double Sample(double x, double y);
    // need to configure intensity range
    // need to configure turbulence when sampling
    // need to configure size of cell samples
    // need to configure sigma of resultant cells
    // need to configure sampling scale

    // distribution for height
    double intensity_min = 0.5;
    double intensity_max = 1.2;
    
    // size of hill cells before scale
    double cell_size = 1.0;

    // max sigma for hill falloff
    double hill_sigma = 0.3;

    // displacement component for generation before scale
    noise::TurbulentDisplace displacement;

    // scatter
    double scatter = 0.5;

    // offset
    glm::dvec2 offset = glm::dvec2(0.0);

    // scale applied to sample points
    glm::dvec2 scale = glm::dvec2(1.0);

    // probability of filling cell
    double fill_probability = 0.4;

    // clamp scatter 0.0 - 1.0 because i don't see the point otherwise
    // sigma will affect sampling width - not gonna bother clamping i dont think
   private:
    // assist in rng; seeding is super expensive :3
    std::mt19937_64 engine;
    // need to cache generation results somewhere
    std::unordered_map<glm::ivec2, _impl::HillInfo> hill_cache;

    // looks up hill params, generating if necessary
    const _impl::HillInfo& LookupHill(const glm::ivec2& offset);
  };
}

#endif // HILL_GENERATOR_H_