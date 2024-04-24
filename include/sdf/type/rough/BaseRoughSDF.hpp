// tough - because the rough should be *generally* contiguous
// for now: just worry about our joined points (in the future, we'll put something behind them)

#ifndef BASE_ROUGH_SDF_H_
#define BASE_ROUGH_SDF_H_

#include "sdf/CPPBundle.hpp"

#include <memory>
namespace mgc {
  namespace rough {
    class BaseRoughSDF {
     public:
      BaseRoughSDF(
        const std::vector<std::shared_ptr<CPPBundle>>& bundles
      );

      // gets bundle assc'd with a given floor index, if available
      // else, returns nullptr
      std::shared_ptr<CPPBundle> GetBundle(size_t index) const;
      double Sample(double x, double y) const;

      size_t Bundles() const;

     private:
      std::vector<std::shared_ptr<CPPBundle>> bundles;
    };
  }
}

#endif // BASE_ROUGH_SDF_H_
