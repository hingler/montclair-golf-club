#ifndef terrain_data_H_
#define terrain_data_H_

#include "course/terrain_type.hpp"


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

  terrain_data operator*(terrain_data lhs, float rhs);

  terrain_data operator+(terrain_data lhs, const terrain_data& rhs);
}

// want to work on speeding up boot if we can
// - async mesh gen (wrapper, like we did before)
// - can save it for later - in either case though, want to accelerate


// - would like to port mesh gen over to the flow we have, but not sure if its doable
// - (can move towards only providing gen factory, generator, and chunker - the rest is implicit)
// - need to come up with a better approach to mesh gen - would like to "step up" mesh res if we can
// - (in an ideal world: mesh would scale up over time, and vertices would "shift" to match new data)
// - idea 1: provide lower-fidelity height data as an instance param (esp if its already generated)

// what's next?
// - want to work on gameplay now
//   - ui logic + polishing it up
//   - walk up to ball and hit it
// - need to slightly modify tree logic (avoid gaps in fairway)

// - tba: figure out splats for hills, mountains, etc
// - way down the line: grass??
// - (works with multimesh - i guess the issue was res, it looks like bottleneck isn't necc. linear???)

#endif