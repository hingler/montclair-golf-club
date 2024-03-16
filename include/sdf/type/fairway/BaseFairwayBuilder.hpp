#ifndef BASE_FAIRWAY_BUILDER_H_
#define BASE_FAIRWAY_BUILDER_H_

#include "sdf/SDFBuilderType.hpp"

#include "sdf/type/fairway/BaseFairwaySDF.hpp"
#include "seed/direct/impl/SimplexHeightFunc.hpp"

namespace mgc {
  namespace fairway {
    // base SDF for fairway
    class BaseFairwayBuilder : public SDFBuilderType<BaseFairwaySDF> {
     public:
      BaseFairwayBuilder();
      std::shared_ptr<BaseFairwaySDF> Get(
        const CourseBundle& bundle,
        std::mt19937_64& engine
      ) const override;

      // for this:
      // - would like to have more info on the box position
      private:
       _impl::SimplexHeightFunc sampler;
       void CreateCapsules(
         const CourseBundle& bundle,
         std::mt19937_64& engine,
         const std::shared_ptr<Bundle>& output
       ) const;
    };
  }
}

#endif // BASE_FAIRWAY_BUILDER_H_
