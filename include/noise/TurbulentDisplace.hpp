#ifndef TURBULENT_DISPLACE_H_
#define TURBULENT_DISPLACE_H_

#include <glm/glm.hpp>

#include <random>

namespace noise {
  class TurbulentDisplace {
   public:
    TurbulentDisplace();

    TurbulentDisplace(const TurbulentDisplace& other);
    TurbulentDisplace& operator=(const TurbulentDisplace& other);
    
    /**
     * @brief Re-seeds this displace field
     * 
     * @param seed - seed used to power sampling locations
     */
    void Seed(size_t seed);

    /**
     * @brief Samples this displacement field at the specified point
     * 
     * @param point - point we wish to sample at
     * @return glm::vec2 - the inputted point, displaced.
     */
    glm::dvec2 Sample(const glm::dvec2& point);

    // size of displacement along each axis.
    glm::dvec2 intensity = glm::dvec2(1.0);

    // scale of underlying noise samples
    glm::dvec2 noise_scale = glm::dvec2(1.0);

    size_t octaves = 1;

   private:
    std::mt19937_64 engine;
    std::uniform_real_distribution<float> offset_dist = std::uniform_real_distribution(-1024.0f, 1024.0f);

    // offsets applied to x and y sampling, based on seed.
    glm::dvec3 offset_x = glm::dvec3(0.0);
    glm::dvec3 offset_y = glm::dvec3(0.0);

    
  };
}

// hill sampling
// - can't place hills too regularly!!! need to move around centers a bunch
// - thinking we shift it around mostly-randomly, calculate distance-to-edge for heights???

#endif // TURBULENT_DISPLACE_H_