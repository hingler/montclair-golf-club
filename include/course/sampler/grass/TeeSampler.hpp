#ifndef TEE_SAMPLER_H_
#define TEE_SAMPLER_H_

// sampler which generates a tee box
#include "course/path/CoursePath.hpp"
#include "course/sampler/ISampler.hpp"

namespace course {
  namespace sampler {
    namespace grass {
      class TeeSampler : public ISampler<float> {
      public:
        TeeSampler(const path::CoursePath& course_path);
        float Sample(float x, float y) const override;
        float width;
        float length;

      private:
        glm::vec2 center;
        glm::vec2 direction_facing;
        glm::vec2 direction_cross;
      };
    }
  }
}

#endif // TEE_SAMPLER_H_