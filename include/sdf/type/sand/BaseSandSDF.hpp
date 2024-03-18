#ifndef BASE_SAND_SDF_H_
#define BASE_SAND_SDF_H_

#include "sdf/CPPBundle.hpp"

#include <memory>

namespace mgc {
  namespace sand {
    class BaseSandSDF {
     public:
      BaseSandSDF(
        const std::shared_ptr<CPPBundle>& traps
      );

      double Sample(double x, double y) const;

      std::shared_ptr<CPPBundle> GetBundle();
     private:
      const std::shared_ptr<CPPBundle> traps;
    };
  }
}

#endif // BASE_SAND_SDF_H_
