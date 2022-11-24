#ifndef TURBULENT_DISPLACE_SAMPLER_H_
#define TURBULENT_DISPLACE_SAMPLER_H_

#include "course/sampler/ISampler.hpp"
#include "course/sampler/SimplexNoiseSampler.hpp"

#include <memory>

namespace course {
  namespace sampler {
    /**
     * @brief Sampler which uses simplex noise to distort an inputted image
     */
    template <typename DataType>
    class TurbulentDisplaceSampler : public ISampler<DataType> {
    public:
      TurbulentDisplaceSampler(
        std::shared_ptr<ISampler<DataType>> input,
        const glm::vec3& scale,
        int octaves
      ) : noise_(std::make_shared<SimplexNoiseSampler>(scale, octaves)),
          image_(input),
          displacement_factor(0.5f) {}

      void SetScale(const glm::vec3& scale) {
        noise_->scale = scale;
      }

      void SetOctaves(int octaves) {
        noise_->octaves = octaves;
      }


      DataType Sample(float x, float y) const override {
        noise_->time = time - 14.2857142857;
        float sample_x = noise_->Sample(x, y);
        noise_->time = time + 14.2857142857;
        float sample_y = noise_->Sample(x, y);
        x += sample_x * displacement_factor;
        y += sample_y * displacement_factor;

        return image_->Sample(x, y);
      }

      // evolution param
      double time;

      // amount to displace by
      float displacement_factor;

    private:
      std::shared_ptr<SimplexNoiseSampler> noise_;
      std::shared_ptr<ISampler<DataType>> image_;

    };
  }
}

#endif // TURBULENT_DISPLACE_SAMPLER_H_