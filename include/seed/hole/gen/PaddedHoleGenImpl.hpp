#ifndef PADDED_HOLE_GEN_IMPL_H_
#define PADDED_HOLE_GEN_IMPL_H_

#include "seed/GrowConfig.hpp"
#include "seed/hole/gen/HoleGenerator.hpp"

#include "seed/hole/overlap/OverlapTester.hpp"

namespace mgc {
  template <typename HeightMap>
  class PaddedHoleGenImpl  : public HoleGenerator {
   public:
    PaddedHoleGenImpl(
      const std::shared_ptr<HeightMap> underlying,
      const std::shared_ptr<OverlapTester> tester,
      const GrowConfig& config_grow
    ) : terrain(underlying), tester(tester), config(config_grow) {

    }
   private:
    const std::shared_ptr<HeightMap> terrain;
    const std::shared_ptr<OverlapTester> tester;
    const GrowConfig config;
  };
}

#endif // PADDED_HOLE_GEN_IMPL_H_
