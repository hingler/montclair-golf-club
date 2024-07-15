#ifndef COURSE_SUB_SAMPLER_H_
#define COURSE_SUB_SAMPLER_H_

#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/sampler/SmoothingMultiBoxSampler.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/gen/FeatureSubSampler.hpp"

namespace mgc {
  namespace impl {
    template <typename HeightType>
    class CourseSubSampler {
     public:
      // tba: generalize this st we can use course sub sampler for everything???
      typedef cg::SmoothingMultiBoxSampler<cg::BaseSmoothingSamplerBox> box_sampler_type;

      CourseSubSampler(
        box_sampler_type&& box,
        const std::shared_ptr<HeightType>& height,
        const std::shared_ptr<mgc_course::mgc_gen::FeatureSubSampler>& pre_gen,
        const std::shared_ptr<mgc_course::mgc_gen::FeatureSubSampler>& post_gen
      ) : box_sampler(box), base_height(height), pre_gen(pre_gen), post_gen(post_gen) {}

      double Sample(double x, double y) const {
        double underlying = base_height->Sample(x, y);
        underlying += pre_gen->SampleHeight(x, y, underlying);
        double course = box_sampler.SampleHeight(x, y, underlying);
        double post = post_gen->SampleHeight(x, y, (course + underlying));
        return underlying + course + post;
      }

      double Get(double x, double y) const {
        return Sample(x, y);
      }

      glm::vec4 SampleSplat(double x, double y, double index) const {
        // (tba) (i dont know why i wrote this hehe)
        return pre_gen->SampleSplat(x, y, index) + box_sampler.SampleSplat(x, y, index) + post_gen->SampleSplat(x, y, index);
      }

      float SampleGrassFill(double x, double y) const {
        float falloffs = std::max(std::min(box_sampler.GetFalloffWeight(x, y), 1.0f), 0.0f);
        float grass_pre = std::clamp(pre_gen->SampleGrassFill(x, y), 0.0f, 1.0f);
        float final_course = (1.0f - falloffs) + falloffs * box_sampler.SampleGrassFill(x, y);
        float grass_post = std::clamp(post_gen->SampleGrassFill(x, y), 0.0f, 1.0f);
        return std::min(final_course, std::min(grass_pre, grass_post));
      }

      float SampleTreeFill(double x, double y) const {
        float falloffs = std::max(std::min(box_sampler.GetFalloffWeight(x, y), 1.0f), 0.0f);
        float tree_pre = std::clamp(pre_gen->SampleTreeFill(x, y), 0.0f, 1.0f);
        float final_course = (1.0f - falloffs) + (falloffs * box_sampler.SampleTreeFill(x, y));
        float tree_post = std::clamp(post_gen->SampleTreeFill(x, y), 0.0f, 1.0f);
        return std::min(final_course, std::min(tree_pre, tree_post));
      }

     private:
      box_sampler_type box_sampler;
      std::shared_ptr<HeightType> base_height;
      std::shared_ptr<mgc_course::mgc_gen::FeatureSubSampler> pre_gen;
      std::shared_ptr<mgc_course::mgc_gen::FeatureSubSampler> post_gen;
    };
  }
}

#endif // COURSE_SUB_SAMPLER_H_
