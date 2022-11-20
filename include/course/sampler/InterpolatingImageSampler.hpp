#ifndef INTERPOLATING_IMAGE_SAMPLER_H_
#define INTERPOLATING_IMAGE_SAMPLER_H_

#include "course/sampler/ISampler.hpp"
#include "image/GenericImage.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <type_traits>

namespace course {
  namespace sampler {
    /**
     * @brief Samples height data from an image, with interpolation
     */

    namespace _traits {
      struct interpolatable_impl {
        template <typename DataType,
        typename AddOp = decltype(std::declval<DataType&>() + std::declval<DataType&>()),
        typename ScaleOp = decltype(std::declval<DataType&>() * 5.0f),
        typename DefaultOp = std::is_default_constructible<DataType>>
        static std::true_type test(int);

        template <typename DataType, typename...>
        static std::false_type test(...);
      };

      template <typename T>
      struct interpolatable : decltype(interpolatable_impl::test<T>(0)) {};
    }

    template <typename DataType>
    class InterpolatingImageSampler : public ISampler<DataType> {
      // ensure we can add, scale, etc...
      static_assert(_traits::interpolatable<DataType>::value);

    public:
      InterpolatingImageSampler(
        std::shared_ptr<image::GenericImage<DataType>> image,
        const glm::vec2& terrain_dims
        ) : image_(image) {
        auto dims = image->Dimensions();
        auto image_dims_vec = glm::vec2(dims.width, dims.height);
        scale_factor_ = glm::vec2(image_dims_vec / terrain_dims);
      }

      DataType Sample(float x, float y) const override {
        // 0, 0 is bottom left corner of image
        const auto IMAGE_FLOOR = glm::ivec2(0, 0);
        auto dims = image_->Dimensions();
        const auto IMAGE_DIMS_CEIL = glm::ivec2(dims.width - 1, dims.height - 1);

        auto image_coords = glm::vec2(x, y) * scale_factor_ - glm::vec2(0.5, 0.5);

        // sample four-wise
        // clamp to ensure we remain in bounds
        // note: if OOB these will sometimes contain erroneous coord data
        // however the clamp op on interp factor should ensure they're not read
        auto ff = glm::clamp(static_cast<glm::ivec2>(glm::floor(image_coords)), IMAGE_FLOOR, IMAGE_DIMS_CEIL);
        auto cf = glm::clamp(ff + glm::ivec2(1, 0), IMAGE_FLOOR, IMAGE_DIMS_CEIL);
        auto fc = glm::clamp(ff + glm::ivec2(0, 1), IMAGE_FLOOR, IMAGE_DIMS_CEIL);
        auto cc = glm::clamp(ff + glm::ivec2(1, 1), IMAGE_FLOOR, IMAGE_DIMS_CEIL);

        float x_t = glm::clamp(image_coords.x - ff.x, 0.0f, 1.0f);
        float y_t = glm::clamp(image_coords.y - ff.y, 0.0f, 1.0f);

        // a bit risky but will always be in bounds after clamp call
        DataType sample_acc = *image_->Get(ff.x, ff.y) * (1.0 - x_t) * (1.0 - y_t);
        sample_acc += *image_->Get(cf.x, cf.y) * x_t * (1.0 - y_t);
        sample_acc += *image_->Get(fc.x, fc.y) * (1.0 - x_t) * y_t;
        sample_acc += *image_->Get(cc.x, cc.y) * x_t * y_t;

        return sample_acc;
      }
    private:
      std::shared_ptr<image::GenericImage<DataType>> image_;
      glm::vec2 scale_factor_;
    };
  }
}

#endif // INTERPOLATING_IMAGE_SAMPLER_H_