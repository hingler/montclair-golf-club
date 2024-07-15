#ifndef MGC_BLANK_WORLD_FEATURE_H_
#define MGC_BLANK_WORLD_FEATURE_H_

#include "corrugate/sampler/DataSampler.hpp"
#include "course/feature/WorldFeature.hpp"
namespace mgc {
  namespace feature {
    namespace generic {
      class BlankWorldFeature : public mgc_course::WorldFeature {
       public:
        BlankWorldFeature(const glm::dvec2& position) : BlankWorldFeature(position - 0.5, glm::dvec2(1.0), 0.0, 0.0) {}
        BlankWorldFeature(const glm::dvec2& origin, const glm::dvec2& size, double rotation, double scale) : SamplerBox(
          origin,
          size
        ), rotation_degrees(rotation), scale(scale) {}

        float SampleHeight(double x, double y) const override { return 0.0f; }
        glm::vec4 SampleSplat(double x, double y, size_t index) const override { return glm::vec4(0.0f); }
        float SampleTreeFill(double x, double y) const override { return 0.0f; }
        float SampleGrassFill(double x, double y) const override { return 0.0f; }

        size_t WriteHeight(   const glm::dvec2& origin, const glm::ivec2& sample_dims, double scale,                float* output,      size_t n_bytes) const override { return 0; }
        size_t WriteSplat(    const glm::dvec2& origin, const glm::ivec2& sample_dims, double scale, size_t index,  glm::vec4* output,  size_t n_bytes, const cg::DataSampler<float>* falloffs) const override { return 0; }
        size_t WriteTreeFill( const glm::dvec2& origin, const glm::ivec2& sample_dims, double scale,                float* output,      size_t n_bytes, const cg::DataSampler<float>* falloffs) const override { return 0; }
        size_t WriteGrassFill(const glm::dvec2& origin, const glm::ivec2& sample_dims, double scale,                float* output,      size_t n_bytes) const override { return 0; }

        float GetSmoothDelta(double x, double y, double underlying) const override { return 0.0f; }
        size_t WriteSmoothDelta(
          const glm::dvec2& origin,
          const glm::ivec2& sample_dims,
          double scale,
          const cg::DataSampler<float>& underlying_data,
          const cg::DataSampler<float>& falloff_sums,
          float* output,
          size_t n_bytes
        ) const override { return 0; }

        double SampleOcclusion(double x, double y) const override {
          return std::numeric_limits<double>::max();
        }

        glm::dvec2 GetFeatureCenter() const override { return GetOrigin() + (GetSize() / 2.0); }
        double GetRotationDegrees() const override { return 0.0; }
        double GetScale() const override { return 1.0; }
       private:
        double rotation_degrees;
        double scale;
      };
    }
  }
}

#endif // MGC_BLANK_WORLD_FEATURE_H_
