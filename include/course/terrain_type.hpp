#ifndef TERRAIN_TYPE_H_
#define TERRAIN_TYPE_H_

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
}

#endif // TERRAIN_TYPE_H_