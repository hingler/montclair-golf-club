#ifndef COURSE_SCHEMA_H_
#define COURSE_SCHEMA_H_

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
  enum terrain_type {
    Fairway,
    Green,
    Rough,
    Sand,
    Path,
    HeavyRough,
    OutOfBounds,
    // used by iterators
    NUM_ELEMENTS
  };

  struct course_schema {
    float terrain[NUM_ELEMENTS];
    /**
     * @brief normalizes this course schema.
     * 
     * @return reference to self
     */
    course_schema& normalize();

    /**
     * @brief raises this course schema to a given power
     * 
     * @param exp - exponent
     * @return reference to self
     */
    course_schema& power(float exp);

    /**
     * @brief Sets a given terrain field to a particular value.
     * 
     * @param index 
     * @return float 
     */
    float get(int index);
  };

  // redesign
  // we'll probably expand this later
  // use an enum to map to array elements
  // perform operations based on that
}

#endif