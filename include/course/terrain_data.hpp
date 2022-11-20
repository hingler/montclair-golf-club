#ifndef terrain_data_H_
#define terrain_data_H_

#include "course/terrain_type.hpp"

// - create simple terrain printer
// - how do we want to do it?
// - interpret a struct containing terrain weights
//  - sand
//  - rough
//  - fairway (+ fringe)
//  - green
//  - heavy rough?
//  - path?
//  - oob terrain?

/**
 * @brief Struct representation of a golf hole, as intensitites.
 */

namespace course {
  struct terrain_data {
    float terrain[NUM_ELEMENTS];
    /**
     * @brief normalizes this course schema.
     * 
     * @return reference to self
     */
    terrain_data& normalize();

    /**
     * @brief raises this course schema to a given power
     * 
     * @param exp - exponent
     * @return reference to self
     */
    terrain_data& power(float exp);

    /**
     * @brief scales terrain_data by a scalar value
     * 
     * @param mag - magnitude to scale by
     * @return reference to self
     */
    terrain_data& scale(float mag);

    /**
     * @brief Sets a given terrain field to a particular value.
     * 
     * @param index 
     * @return float 
     */
    float get(int index);

    terrain_data& operator+=(const terrain_data& rhs);
    terrain_data& operator*=(float rhs);
  };

  // note because ive never used this :)
  // constexpr is implicitly inline
  constexpr terrain_data operator*(terrain_data lhs, float rhs) {
    return lhs *= rhs;
  }

  constexpr terrain_data operator+(terrain_data lhs, const terrain_data& rhs) {
    return lhs += rhs;
  }
}

#endif