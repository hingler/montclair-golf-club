#ifndef MGC_OCCLUDING_SAMPLER_H_
#define MGC_OCCLUDING_SAMPLER_H_

#include <memory>

namespace mgc {
  namespace terrain {
    template <typename SamplerType>
    class OccludingSampler {
     public:
      OccludingSampler(
        const std::shared_ptr<SamplerType>& sampler
      ) : sampler(sampler) {}

      double Sample(double x, double y) const {
        return -(sampler->Sample(x, y));
      }

     private:
      std::shared_ptr<SamplerType> sampler;
    };
  }
}

#endif // MGC_OCCLUDING_SAMPLER_H_
