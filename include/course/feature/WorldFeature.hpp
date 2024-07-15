#ifndef MGC_WORLD_FEATURE_H_
#define MGC_WORLD_FEATURE_H_

#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/box/LocalSmoothTerrainBox.hpp"
#include "corrugate/sampler/DataSampler.hpp"
#include "corrugate/sampler/smooth/LocalizedSmoother.hpp"
#include "course/feature/FeatureSpec.hpp"
#include "course/feature/TransformSampler.hpp"
#include "glm/gtx/rotate_vector.hpp"
namespace mgc {
  namespace mgc_course {
    // represents a world feature
    // (note: tree + grass maps represent occlusion, instead of raw value)

    class WorldFeature : public cg::BaseSmoothingSamplerBox {
     public:
      // Sample occlusion map - return dist to occlusion SDF
      virtual double SampleOcclusion(double x, double y) const = 0;
      // returns center of feature
      virtual glm::dvec2 GetFeatureCenter() const = 0;

      virtual double GetRotationDegrees() const = 0;
      virtual double GetScale() const = 0;
      FeatureSpec GetFeatureSpec() const {
        FeatureSpec res;
        res.position = GetFeatureCenter();
        res.rotation_degrees = GetRotationDegrees();
        res.scale = GetScale();

        return res;
      }
    };

    template <
      typename BaseType,
      typename SmootherSDF,
      typename OcclusionSampler
    >
    class WorldFeatureImpl : public WorldFeature {
      public:
      template <
        typename HeightType,
        typename SplatType,
        typename FillType,
        typename GrassFillType
      >
      WorldFeatureImpl(
        const cg::FeatureBox& base,
        const std::shared_ptr<HeightType>& heightmap,
        const std::shared_ptr<SplatType>& splat,
        const std::shared_ptr<FillType>& fill,
        const std::shared_ptr<GrassFillType>& grass_fill,
        const std::shared_ptr<cg::smooth::LocalizedSmoother<BaseType, SmootherSDF>>& smoother,
        const std::shared_ptr<OcclusionSampler>& occlusion_sampler,
        double rotation_degrees = 0.0,
        double scale = 1.0
      ) : SamplerBox(
        base.GetOrigin(),
        base.GetSize()
      ), base_box(
        base,
        heightmap,
        splat,
        fill,
        grass_fill,
        smoother
      ), rotation_degrees(rotation_degrees), scale(scale),
      occlusion_sampler(occlusion_sampler)
      {}

      // rotating here makes sense to me

      // note: since tree map is "occluding" do we want to use some other work around to pass raw samplers in?
      float SampleHeight(double x, double y) const override {
        return base_box.SampleHeight(x, y);
      }

      glm::vec4 SampleSplat(double x, double y, size_t index) const override {
        return base_box.SampleSplat(x, y, index);
      }

      float SampleTreeFill(double x, double y) const override {
        return base_box.SampleTreeFill(x, y);
      }

      float SampleGrassFill(double x, double y) const override {
        return base_box.SampleGrassFill(x, y);
      }

      double SampleOcclusion(double x, double y) const override {
        // glm::dvec2 point_local = glm::dvec2(x, y) - GetOrigin();
        // return occlusion_sampler->Sample(point_local.x, point_local.y);
        // could this be the issue???
        return 1.0;
      }

      glm::dvec2 GetFeatureCenter() const override {
        return GetOrigin() + (GetSize() / 2.0);
      }

      size_t WriteHeight(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        float* output,
        size_t n_bytes
      ) const override {
        return base_box.WriteHeight(origin, sample_dims, scale, output, n_bytes);
      }

      size_t WriteSplat(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        size_t index,
        glm::vec4* output,
        size_t n_bytes,
        const cg::DataSampler<float>* falloffs
      ) const override {
        return base_box.WriteSplat(origin, sample_dims, scale, index, output, n_bytes, falloffs);
      }

      size_t WriteTreeFill(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        float* output,
        size_t n_bytes,
        const cg::DataSampler<float>* falloffs
      ) const override {
        return base_box.WriteTreeFill(origin, sample_dims, scale, output, n_bytes, falloffs);
      }

      size_t WriteGrassFill(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        float* output,
        size_t n_bytes
      ) const override {
        return base_box.WriteGrassFill(origin, sample_dims, scale, output, n_bytes);
      }

      float GetSmoothDelta(double x, double y, double underlying) const override {
        return base_box.GetSmoothDelta(x, y, underlying);
      }

      size_t WriteSmoothDelta(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        const cg::DataSampler<float>& underlying_data,
        const cg::DataSampler<float>& falloff_sums,
        float* output,
        size_t n_bytes
      ) const override {
        return base_box.WriteSmoothDelta(origin, sample_dims, scale, underlying_data, falloff_sums, output, n_bytes);
      }

      double GetRotationDegrees() const override { return rotation_degrees; }
      double GetScale() const override { return scale; }

     private:
      const double rotation_degrees;
      const double scale;
      cg::LocalSmoothTerrainBox<BaseType, SmootherSDF> base_box;
      std::shared_ptr<OcclusionSampler> occlusion_sampler;

    };
  }
}
// places a concretized world feature onto our map
// - use multisampler to bound

// how should it work??
// - for clubhouses: we want to place the clubhouse near our course seeds (how do we know where those are??)
// - (alt: how can we know where they are, before we place the seeds?)
//

// implement in two parts
// TerrainFeature (things which are stamped in place prior to course gen)
// CourseFeature (things which are stamped in place after course gen, particularly rel to courses)
//
// TerrainFeatures:
// - monuments and buildings
// - villages
// - clubhouse
//   - for each chunk: choose one "hub" for our courses
//     - (at or near a seed point)
//   - map will extend to all holes generated from that hub
// - geological features (lakes, water, etc.)
// inputs
// - height maps
// - *some* amount of seed data
//
// CourseFeatures
// - tee markers, lights, flags
// - probably walking paths
// - possibly roads? (would have to be connected to certain building systems and some such)
// - (anything that should go on/around the courses/buildings)
//
// note: need some terrain features which are placed BEFORE seed path is drawn
// - if we know where seed paths are going tyo start from, then it's no problem!
// - (we can definitely do that)

#endif // MGC_WORLD_FEATURE_H_
