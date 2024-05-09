#ifndef SDF_THRESHOLD_SAMPLER_H_
#define SDF_THRESHOLD_SAMPLER_H_

#include <memory>

namespace mgc {
  template <typename SDFType>
  class SDFThresholdSampler {
   public:
    SDFThresholdSampler(std::shared_ptr<SDFType> sdf) : SDFThresholdSampler(sdf, 0.0) {}

    SDFThresholdSampler(const std::shared_ptr<SDFType>& sdf, double t) : sdf(sdf), threshold(t) {}

    double Sample(double x, double y) const {
      return (sdf->Sample(x, y) <= threshold ? 1.0 : 0.0);
    }
   private:
    std::shared_ptr<SDFType> sdf;
    double threshold;
  };
}

#endif // SDF_THRESHOLD_SAMPLER_H_
