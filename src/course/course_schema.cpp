#include "course/course_schema.hpp"

#include <cmath>

namespace course {
  course_schema& course_schema::normalize() {
    float factor = 0;
    float cur;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      cur = terrain[i];
      factor += cur * cur;
    }


    if (abs(factor) <= 0.000001) {
      factor = 0.0;
    } else {
      factor = 1.0f / sqrt(factor);
    }

    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] *= factor;
    }

    return *this;
  }

  course_schema& course_schema::power(float exp) {
    float cur;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
      terrain[i] = pow(terrain[i], exp);
    }

    return *this;
  }

  float course_schema::get(int index) {
    return (index >= 0 && index < NUM_ELEMENTS ? terrain[index] : -1.0f);
  }


}