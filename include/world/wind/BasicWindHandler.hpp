#ifndef MGC_BASIC_WIND_HANDLER_H_
#define MGC_BASIC_WIND_HANDLER_H_

#include "gog43/Logger.hpp"
#include "world/WindHandler.hpp"
#include <memory>
namespace mgc {
  namespace world {
    // noisetype: scalar vector field defined in XY space.
    template <typename NoiseType>
    struct BasicWindHandler : public WindHandler {
      BasicWindHandler(
        const std::shared_ptr<NoiseType>& noise
      ) : noise(noise) {}

      glm::dvec3 GetWind(const glm::dvec3& position) const override {
        // calc gradient
        glm::dvec2 p2(position.x, position.z);

        double sample_base = noise->Sample(p2.x, p2.y);
        double sample_x = noise->Sample(p2.x + WIND_EPSILON, p2.y);
        double sample_y = noise->Sample(p2.x, p2.y + WIND_EPSILON);

        double grad_dx = (sample_x - sample_base) / WIND_EPSILON;
        double grad_dy = (sample_y - sample_base) / WIND_EPSILON;

        glm::dvec2 grad_curl(-grad_dy, grad_dx);

        gog43::print("wind sample at ", p2.x, ", ", p2.y, ": ", grad_curl.x, ", ", grad_curl.y);

        // cool!
        return glm::dvec3(grad_curl.x, 0.0, grad_curl.y);
      }
     private:
      std::shared_ptr<NoiseType> noise;
      static constexpr double WIND_EPSILON = 0.01;
    };
  }
}

#endif // MGC_BASIC_WIND_HANDLER_H_
