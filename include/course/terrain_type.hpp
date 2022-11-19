#ifndef TERRAIN_TYPE_H_
#define TERRAIN_TYPE_H_

namespace course {
  // reorder
  enum terrain_type {
    Sand,
    Green,
    Path,
    Fairway,
    Rough,
    HeavyRough,
    OutOfBounds,
    // used by iterators
    NUM_ELEMENTS
  };
}

#endif // TERRAIN_TYPE_H_