#ifndef SDF_BUILDER_TYPE_H_
#define SDF_BUILDER_TYPE_H_

#include "path/CourseBundle.hpp"
#include <memory>
#include <random>
namespace mgc {

  template <typename SDFType>
  class SDFBuilderType {
   public:
    typedef SDFType sdf_type;
    // returns SDF for a given terrain type, for a given hole
    // pass in an engine to use here?
    virtual std::shared_ptr<SDFType> Get(
      const CourseBundle& bundle,
      std::mt19937_64& engine
    ) const = 0;
  };
}

#endif // SDF_BUILDER_TYPE_H_
