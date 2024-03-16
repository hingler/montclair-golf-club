#ifndef HOLE_GENERATOR_H_
#define HOLE_GENERATOR_H_

#include <memory>

#include "seed/hole/HoleBox.hpp"
#include "seed/hole/SDFHoleBox.hpp"

#include <corrugate/MultiSampler.hpp>

// maintains a bunch of hole boxes
namespace mgc {
  // virtual interface for gen
  class HoleGenerator {
   public:
    // return a different type
    virtual std::unique_ptr<SDFHoleBox> GenerateHole(const HoleBox& box) = 0;
  };
}

// https://www.shadertoy.com/view/ws3XRf

#endif // HOLE_GENERATOR_H_
