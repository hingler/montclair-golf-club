#ifndef SDF_SMOOTHER_CPP_H_
#define SDF_SMOOTHER_CPP_H_

#include <sdf_jak/sdf_bundle.h>

#include "sdf/Bundle.hpp"

namespace mgc {
  class Smoother {
   public:
    Smoother(Bundle& a, Bundle& b, double factor);
    double Sample(double x, double y);
    ~Smoother();

    Smoother(const Smoother& other) = delete;
    Smoother& operator=(const Smoother& other) = delete;

    Smoother(Smoother&& other);
    Smoother& operator=(Smoother&& other);
   private:
    SDFSmoother* smoother;
  };
}

#endif // SDF_SMOOTHER_CPP_H_
