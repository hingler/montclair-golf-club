#ifndef FAIRWAY_TERRAIN_H_
#define FAIRWAY_TERRAIN_H_

namespace mgc {
  // issue: seems like it breaks the "inherent promise"
  // - goal would be to return a map that we can just "plug in" no issues
  class FairwayTerrain : public HoleTerrain {
    // fairway first - return bundle
    // then create sand - get bundle, copy, and invert
    // then create green - pad, add to fairway - then, place on top

    // - s subtract sand from fairway
    // - expand green and smooth onto fairway
    // - try to trim fairway down to 0, near green

    // oops lol - solid test will pass for fairway, but sdf will be invalid



    // idea:
    // - thin path - from tee box to green
    // on top:
    // - thicker path - pick up a bit before the first "stop"
    // - add some greebles on top
    // - smooth the whole thing
  };

  // idea1:
  // - base terrain creator returns a base fairway splat, sand splat, and green splat
  // - then: we pass that component here, and we calc height/splat from those parts
}

#endif // FAIRWAY_TERRAIN_H_
