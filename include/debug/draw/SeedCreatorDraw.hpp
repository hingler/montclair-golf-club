#ifndef SEED_CREATOR_DRAW_H_
#define SEED_CREATOR_DRAW_H_

#include "seed/SeedBasedHoleCreator.hpp"
namespace mgc {

  // debug draw for seed creator
  template <typename HeightMap>
  class SeedCreatorDraw {
    typedef std::shared_ptr<SeedBasedHoleCreator<HeightMap>> c_type;
   public:
    SeedCreatorDraw(
      const c_type& creator,
      size_t index,
      size_t color_idx
    ) : creator(creator), index(index), color(color_idx) {
      assert(color > 0 && color < 4);
    }

    double Sample(double x, double y) const {
      glm::vec4 creator_sample = creator->SampleSplat(x, y, index);
      return creator_sample[color];
    }
   private:
    c_type creator;
    const size_t index;
    const size_t color;
  };
}

#endif // SEED_CREATOR_DRAW_H_
