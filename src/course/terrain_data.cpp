#include "course/terrain_data.hpp"

#include <cmath>

namespace course {
  terrain_data& terrain_data::normalize() {
    float factor = 0;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      // l1 norm
      factor += abs(terrain[i]);
    }


    if (abs(factor) <= 0.000001) {
      factor = 0.0;
    } else {
      factor = 1.0f / factor;
    }

    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] *= factor;
    }

    return *this;
  }

  terrain_data& terrain_data::power(float exp) {
    float cur;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] = pow(terrain[i], exp);
    }

    return *this;
  }

  terrain_data& terrain_data::scale(float mag) {
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] *= mag;
    }

    return *this;
  }

  float terrain_data::get(int index) {
    return (index >= 0 && index < NUM_ELEMENTS ? terrain[index] : -1.0f);
  }

  terrain_data& terrain_data::operator+=(const terrain_data& rhs) {
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] += rhs.terrain[i];
    }

    return *this;
  }

  terrain_data& terrain_data::operator*=(float rhs) {
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] *= rhs;
    }

    return *this;
  }

  terrain_data operator*(terrain_data lhs, float rhs) {
    return lhs *= rhs;
  }

  terrain_data operator+(terrain_data lhs, const terrain_data& rhs) {
    return lhs += rhs;
  }
}