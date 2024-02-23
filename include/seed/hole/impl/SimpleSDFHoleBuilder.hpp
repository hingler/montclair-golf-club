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
    std::shared_ptr<SDFHoleBox> CreateSDF(const HoleBox& box, const std::vector<glm::dvec2>& points, double len) {
      // given a box and a local path, create SDFs
      // (convert path to global? eh - sdfs will be local too)
      // calculate "par" (note: we should be taking the seed path we have - don't fudge it!)
      engine.seed(box.seed);

      // keep it ultra-simple for now
      // - don't worry about seeds, or generation behavior - just go with some capsules and some noise
      // - determine par via course length
      // - (hole box should probably store some info like that)
      glm::dvec2 tee_direction = glm::normalize(points[1] - points[0]);

      double fairway_start = FAIRWAY_START(engine);

      Bundle fairway, green, sand;
      fairway.AddCapsule(points.data(), points.size(), 32.0);
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
