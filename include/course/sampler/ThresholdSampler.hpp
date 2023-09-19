#ifndef THRESHOLD_SAMPLER_H_
#define THRESHOLD_SAMPLER_H_

#include <algorithm>
#include <memory>
#include <type_traits>

#include <glm/glm.hpp>

#include "type/sampler_type.hpp"

namespace course {
  namespace sampler {
    /**
     * @brief Clips a pre-existing sampler by some threshold.
     * 
     * @tparam Sampler - sampler type passed in
     */
    template <typename Sampler>
    class ThresholdSampler {
      static_assert(type::sampler_type<Sampler>::value);
      public:
      ThresholdSampler(
        double min,
        double max,
        double smooth,
        const std::shared_ptr<Sampler>& sampler
      ) : min_(min), max_(max), smooth_(smooth), sampler_(sampler) {}

      double Sample(double x, double y) {
        double sample = sampler_->Sample(x, y);

        // distance from clamp region
        double dist = std::max(-std::min(sample - min_, max_ - sample), 0.0);
        return (1.0 - glm::smoothstep(0.0, smooth_, dist)) * intensity;
      }

      double intensity = 1.0;
      private:
      double min_;
      double max_;
      double smooth_;


      std::shared_ptr<Sampler> sampler_;
    };
  }
}

#endif // THRESHOLD_SAMPLER_H_