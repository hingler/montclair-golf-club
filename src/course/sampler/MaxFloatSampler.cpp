#include "course/sampler/MaxFloatSampler.hpp"

#include <algorithm>

namespace course {
  namespace sampler {
    float MaxFloatSampler::Sample(float x, float y) const {
      float acc = 0.0f;

      for (auto& sampler : samplers) {
        acc = std::max(acc, sampler->Sample(x, y));
      }

      return acc;
    }

    void MaxFloatSampler::AddSampler(std::shared_ptr<ISampler<float>> sampler) {
      samplers.push_back(sampler);
    }
  }
}