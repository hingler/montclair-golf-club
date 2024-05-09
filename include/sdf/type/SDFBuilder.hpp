#ifndef SDF_BUILDER_H_
#define SDF_BUILDER_H_

// compiling base SDFs together:
// - subtract green from sand
// - subtract sand (subtracted sand) from fairway
// - add green to fairway (subtracted fairway)
// - add teebox back to fairway at the end

// c -> rust not working - gonna write smoother again in c++ so we can template :3

// green -> bundle
// sand -> ssub<sand, green>
// fairway -> smin<ssub<fairway, sand>, green>

#include "sdf/CPPSmoother.hpp"

#include "sdf/type/fairway/BaseFairwayBuilder.hpp"
#include "sdf/type/green/BaseGreenBuilder.hpp"
#include "sdf/type/rough/BaseRoughBuilder.hpp"
#include "sdf/type/sand/BaseSandBuilder.hpp"
#include "seed/hole/impl/SDFGrow.hpp"

#include <memory>

namespace mgc {
  class SDFBuilder {
    typedef fairway::BaseFairwayBuilder::sdf_type b_fairway;
    typedef green::BaseGreenBuilder::sdf_type b_green;
    typedef sand::BaseSandBuilder::sdf_type b_sand;
    typedef rough::BaseRoughBuilder::sdf_type b_rough;

   public:
    typedef b_green green_type;
    typedef CPPSmoother<SDFGrow<b_green>, b_sand> sand_type;
    typedef CPPSmoother<b_green, CPPSmoother<CPPSmoother<b_sand, b_fairway>, SDFGrow<b_green>>> fairway_type;
    // hehe
    // probably a better way to do all this
    // (modify splatmanager st splats trounce each other in order?? ex. alpha var test)
    typedef b_rough rough_type;

    SDFBuilder(
      const std::shared_ptr<b_fairway>& fairway,
      const std::shared_ptr<b_green>& green,
      const std::shared_ptr<b_sand>& sand,
      const std::shared_ptr<b_rough>& rough
    );

    std::shared_ptr<green_type> GetGreen();
    std::shared_ptr<sand_type> GetSand();
    std::shared_ptr<fairway_type> GetFairway();
    std::shared_ptr<rough_type> GetRough();

   private:
    std::shared_ptr<b_fairway> base_fairway;
    std::shared_ptr<b_sand> base_sand;
    std::shared_ptr<b_green> base_green;
    std::shared_ptr<b_rough> base_rough;

    std::shared_ptr<green_type> green_cache;
    std::shared_ptr<sand_type> sand_cache;
    std::shared_ptr<fairway_type> fairway_cache;
    std::shared_ptr<rough_type> rough_cache;

  };
}

#endif // SDF_BUILDER_H_
