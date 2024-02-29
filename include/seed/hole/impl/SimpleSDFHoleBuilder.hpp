#ifndef SIMPLE_SDF_HOLE_BUILDER_H_
#define SIMPLE_SDF_HOLE_BUILDER_H_

#include "sdf/Bundle.hpp"
#include "seed/hole/SDFHoleBox.hpp"
#include "seed/hole/HoleBox.hpp"

#include <memory>
#include <random>

namespace mgc {
  template <typename HeightType>
  class SimpleSDFHoleBuilder {
   public:
    SimpleSDFHoleBuilder(std::shared_ptr<HeightType> height) : height(height) {}
    template <typename BaseType>
    std::unique_ptr<SDFHoleBox> CreateSDF(const HoleBox& box, const std::vector<glm::dvec2>& points, double len) {
      // tba: now that we have our lib in place, we can jump straight to making proper boxes and converting them!
      engine.seed(box.seed);

      // generation flow
      // - need a base-type wrapper (which generates as we sample)
      // - need similar behavior for smoothing type (generate course terrain as we sample)

      // chunker fulfills this
      // - specifically: when a chunk is sampled, we generate all ungenerated seeds in its vicinity
      // - 3x3 :)

      // tba2: need to move to pulling chunks instead of pulling samples

      // impl
      // - seeds are *roughly* aligned to chunks
      //   - seed scatter: thinking of a generic "hex" pattern
      //   - given a radius, place 1/2 over, sqrt(3)/2 up
      //   - on ones - 1/sqrt(3) over, 1 up
      //   - just precalc all seeds for a given chunk
      // - maintain hole boxes, and proper boxes
      //   - store in chunks, if we can
      //   - fragment course repo per-chunk
      //     - one store for holes, one store for boxes (multisampler - need to fragment further)
      //     - fetches + inserts for each
      //   - create overlap tester which receives multiple
      //   
      // on fetch
      // - generate hole boxes for 3x3 radius
      //   - one component to associate chunks with generated results
      //   - chunks can be stored as simple ivec2 -> (data)
      // - generate sdf -> smoothing box for 1x1
      //   - fetch hole boxes for 3x3 radius (from gen)
      //   - test bounds for containing point
      //   - create out of a list of hole stores, and our present SDF store
      // write paths: find chunk which contains point
      // write sdfs: find all chunks covered

      // store
      // - do we need to store SDF boxes? we only test against them for a single chunk
      // - we do, however, need to store sampler boxes.

      // multisampler for hole chunks (per-chunk, wraps hole multisampler)
      // multisampler for all course terrain (might be good to chunk multisampler)
      // - check if exists before sampling
      // - on sample: fetch course chunks first, map points to local, then multisample contents

      // queueing work
      // - thread pool might be ill-suited - not sure how long path takes (thinking a few ms)
      // - find all "seed candidates" in 3x3 radius and generate for them
      // - for SDFs: generate greedily
      // - test only against SDFs in current cell
      // - test against SDF and hole paths in neighbor cells

      // keep it ultra-simple for now
      // - don't worry about seeds, or generation behavior - just go with some capsules and some noise
      // - determine par via course length
      // - (hole box should probably store some info like that)
      
      // point/chunk -> chunks
      // chunks -> seed lists -> seed chunks
      // sampled chunk -> SDF (one at a time)
      // SDF -> samplers
      glm::dvec2 tee_direction = glm::normalize(points[1] - points[0]);

      // ignore for now
      double fairway_start = FAIRWAY_START(engine);

      auto fairway = std::make_shared<Bundle>();
      auto green = std::make_shared<Bundle>();
      auto sand = std::make_shared<Bundle>();

      fairway->AddCapsule(points.data(), points.size(), 32.0);

      return std::make_unique<SDFHoleBoxImpl<Bundle, Bundle, Bundle>>(
        box,
        fairway,
        green,
        sand
      );
    }
   private:
    std::mt19937_64 engine;
    const std::shared_ptr<HeightType> height;

    // arb numbers
    static std::uniform_real_distribution<double> FAIRWAY_START;
    static constexpr double PAR3_THRESHOLD = 224.5;
    static constexpr double PAR4_THRESHOLD = 436.1;
    // tba: extend further
  };

  template <typename HeightType>
  std::uniform_real_distribution<double> SimpleSDFHoleBuilder<HeightType>::FAIRWAY_START = std::uniform_real_distribution<double>(95.0, 155.0);
}

#endif // SIMPLE_SDF_HOLE_BUILDER_H_
