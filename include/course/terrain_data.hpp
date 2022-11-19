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
     * @brief Sets a given terrain field to a particular value.
     * 
     * @param index 
     * @return float 
     */
    float get(int index);
  };
}

#endif