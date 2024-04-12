#ifndef COURSE_SUB_SAMPLER_H_
#define COURSE_SUB_SAMPLER_H_

#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/sampler/SmoothingMultiBoxSampler.hpp"
namespace mgc {
  namespace impl {
    template <typename HeightType>
    class CourseSubSampler {
     public:
      // tba: generalize this st we can use course sub sampler for everything???
      typedef cg::SmoothingMultiBoxSampler<cg::BaseSmoothingSamplerBox> box_sampler_type;

      CourseSubSampler(
        box_sampler_type&& box,
        const std::shared_ptr<HeightType>& height
      ) : box_sampler(box), base_height(height) {}

      double Sample(double x, double y) const {
        double underlying = base_height->Sample(x, y);
        double course = box_sampler.SampleHeight(x, y, underlying);
        return underlying + course;
      }

      double Get(double x, double y) const {
        return Sample(x, y);
      }

      glm::vec4 SampleSplat(double x, double y, double index) const {
        // (tba)
        return box_sampler.SampleSplat(x, y, index);
      }

      float SampleGrassFill(double x, double y) const {
        float falloffs = std::max(std::min(box_sampler.GetFalloffWeight(x, y), 1.0f), 0.0f);
        return (1.0f - falloffs) + falloffs * box_sampler.SampleGrassFill(x, y);
      }

     private:
      box_sampler_type box_sampler;
      std::shared_ptr<HeightType> base_height;
    };
  }
}

#endif // COURSE_SUB_SAMPLER_H_
