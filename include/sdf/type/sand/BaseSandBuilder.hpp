#ifndef BASE_SAND_BUILDER_H_
#define BASE_SAND_BUILDER_H_

#include "sdf/SDFBuilderType.hpp"
#include "sdf/type/sand/BaseSandSDF.hpp"
namespace mgc {
  namespace sand {
    class BaseSandBuilder : public SDFBuilderType<BaseSandSDF> {
     public:
      std::shared_ptr<BaseSandSDF> Get(
        const CourseBundle& bundle,
        std::mt19937_64& engine
      ) const override;

     private:
    };
  }
}

#endif // BASE_SAND_BUILDER_H_
