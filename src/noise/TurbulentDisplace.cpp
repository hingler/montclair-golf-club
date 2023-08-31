#include "noise/TurbulentDisplace.hpp"

#include <glm/gtc/noise.hpp>

// any irrational num
#define OCTAVE_SCALE_FAC M_SQRT2

namespace noise {
  TurbulentDisplace::TurbulentDisplace() {
    Seed(0);
  }

  void TurbulentDisplace::Seed(size_t seed) {
    engine.seed(seed);

    offset_x.x = offset_dist(engine);
    offset_x.y = offset_dist(engine);
    offset_x.z = offset_dist(engine);
    offset_y.x = offset_dist(engine);
    offset_y.y = offset_dist(engine);
    offset_y.z = offset_dist(engine);
  }


  glm::dvec2 TurbulentDisplace::Sample(const glm::dvec2& point) {
    if (octaves <= 0) {
      return point;
    }
    
    double displacement_x = 0.0;
    double displacement_y = 0.0;

    glm::dvec3 sample_point = glm::dvec3(point.x, point.y, 0.0);

    // using dvec for accuracy here as the offsets will get large
    // any way that we can avoid this div call??
    // leave it for now
    // - set scale via func, store inverse
    glm::dvec3 sample_x = (sample_point + offset_x) / noise_scale.x;
    glm::dvec3 sample_y = (sample_point + offset_y) / noise_scale.y;

    double rolling_intensity = 0.0;

    double current_intensity = 0.5;
    for (size_t i = 0; i < octaves; i++) {
      displacement_x += ((glm::simplex(sample_x) - 0.5) * (intensity.x * current_intensity));
      displacement_y += ((glm::simplex(sample_y) - 0.5) * (intensity.y * current_intensity));
      
      rolling_intensity += current_intensity;
      current_intensity *= 0.5;
      sample_x *= OCTAVE_SCALE_FAC;
      sample_y *= OCTAVE_SCALE_FAC;
    }

    displacement_x /= rolling_intensity;
    displacement_y /= rolling_intensity;

    return glm::dvec2(displacement_x + point.x, displacement_y + point.y);
  }
}