#ifndef MGC_TRANSFORM_SAMPLER_H_
#define MGC_TRANSFORM_SAMPLER_H_

#include "glm/gtx/rotate_vector.hpp"
#include <complex>
#include <memory>

#include <glm/glm.hpp>

namespace mgc {
  namespace mgc_course {
    template <typename SamplerType>
    class TransformSampler {
      public:
      TransformSampler(
        const std::shared_ptr<SamplerType>& sampler,
        double rotation_degrees,
        double scale,
        const glm::dvec2& center
      ) : rotation_rads(glm::radians(rotation_degrees)), scale(scale), center(center), sampler(sampler) {}

      double Sample(double x, double y) const {
        if (std::abs(rotation_rads) < 0.0001) {
          return sampler->Sample(x, y);
        }

        glm::dvec2 center_rel = glm::dvec2(x, y) - center;
        glm::dvec2 sample_pos = glm::rotate(center_rel, -rotation_rads);
        sample_pos += center;

        return sampler->Sample(sample_pos.x, sample_pos.y);
      }

      // transform splatsampler hehe
      private:
      double rotation_rads;
      double scale;
      glm::dvec2 center;

      std::shared_ptr<SamplerType> sampler;
    };
  }
}

#endif // MGC_TRANSFORM_SAMPLER_H_
