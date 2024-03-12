#ifndef BASE_SDF_BUILDER_H_
#define BASE_SDF_BUILDER_H_

#include <memory>
namespace mgc {

  template <typename SDFType>
  class SDFBuilderType {
   public:

    // returns SDF for a given terrain type, for a given hole
    virtual std::shared_ptr<SDFType> Get() = 0;
  };

  // assumes responsibility for generating base SDFs for terrain
  class BaseSDFBuilder {

  };
}

#endif // BASE_SDF_BUILDER_H_
