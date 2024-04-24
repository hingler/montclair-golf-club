#ifndef BASE_ROUGH_BUILDER_H_
#define BASE_ROUGH_BUILDER_H_

#include "path/CourseBundle.hpp"
#include "sdf/SDFBuilderType.hpp"
#include "sdf/type/rough/BaseRoughSDF.hpp"
#include "seed/direct/impl/SimplexHeightFunc.hpp"
#include <random>
namespace mgc {
  namespace rough {
    class BaseRoughBuilder : public SDFBuilderType<BaseRoughSDF> {
     public:
      BaseRoughBuilder();
      std::shared_ptr<BaseRoughSDF> Get(
        const CourseBundle& bundle,
        std::mt19937_64& engine
      ) const override;

     private:
      _impl::SimplexHeightFunc sampler;
      // nothing yet hehe
    };
  }
}

#endif // BASE_ROUGH_BUILDER_H_
