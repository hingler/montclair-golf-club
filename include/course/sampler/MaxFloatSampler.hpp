#ifndef MAX_FILTER_SAMPLER_H_
#define MAX_FILTER_SAMPLER_H_

#include "course/sampler/ISampler.hpp"

#include <memory>
#include <vector>

namespace course {
  namespace sampler {
    class MaxFloatSampler : public ISampler<float> {
    public:
      float Sample(float x, float y) const override;
      void AddSampler(std::shared_ptr<ISampler<float>> sampler);
    private:
      std::vector<std::shared_ptr<ISampler<float>>> samplers;
    };
  }
}

#endif // MAX_FILTER_SAMPLER_H_