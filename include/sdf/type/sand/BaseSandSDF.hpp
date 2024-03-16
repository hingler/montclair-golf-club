#ifndef BASE_SAND_SDF_H_
#define BASE_SAND_SDF_H_

#include "sdf/Bundle.hpp"

#include <memory>

namespace mgc {
  namespace sand {
    class BaseSandSDF {
     public:
      BaseSandSDF(
        const std::shared_ptr<Bundle>& traps
      );

      double Sample(double x, double y) const;

      std::shared_ptr<Bundle> GetBundle();
     private:
      const std::shared_ptr<Bundle> traps;
    };
  }
}

#endif // BASE_SAND_SDF_H_
