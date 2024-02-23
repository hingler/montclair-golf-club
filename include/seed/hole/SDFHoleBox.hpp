#ifndef SDF_HOLE_BOX_H_
#define SDF_HOLE_BOX_H_

#include "corrugate/FeatureBox.hpp"
namespace mgc {
  // specific type
  class SDFHoleBox : public cg::FeatureBox {
  };

  template <typename FT, typename GT, typename ST>
  class SDFHoleBoxImpl : public SDFHoleBox {
    // should be able to convert to something "virtual" here???
  };
}

// how to do this?
// - wrap SDFs in a way that makes them trivial to convert into samplers
// - idea: just come up with a templated spec that can create shit given any SDF

#endif // SDF_HOLE_BOX_H_
