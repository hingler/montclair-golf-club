#ifndef SDF_THRESHOLD_SAMPLER_H_
#define SDF_THRESHOLD_SAMPLER_H_

#include <sdf/Bundle.hpp>

#include <memory>

namespace mgc {
  class SDFThresholdSampler {
   public:
    SDFThresholdSampler(std::shared_ptr<Bundle> sdf) : sdf(sdf) {

    }

    double Sample(double x, double y) const {
      return (sdf->Sample(x, y) <= 0.0 ? 1.0 : 0.0);
    }
   private:
    std::shared_ptr<Bundle> sdf;
  };
}

#endif // SDF_THRESHOLD_SAMPLER_H_