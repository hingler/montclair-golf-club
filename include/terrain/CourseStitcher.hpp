#ifndef COURSE_STITCHER_H_
#define COURSE_STITCHER_H_

#include "chunker/ChunkIdentifier.hpp"
#include "corrugate/sampler/DataSampler.hpp"
#include "corrugate/sampler/SampleWriterGeneric.hpp"
#include "course/feature/gen/WorldFeatureManager.hpp"
#include "seed/ChunkConfig.hpp"
#include "seed/GrowConfig.hpp"
#include "seed/SeedBasedHoleCreator.hpp"
#include "seed/SeedGrower.hpp"
#include "terrain/PostGenHeightmap.hpp"
#include "terrain/impl/CourseSubSampler.hpp"
#include <memory>

namespace mgc {
  // handles the work involved in combining
  // seed behavior with some underlying terrain


  // managing chunk config?
  // managing grow config?

  // eventually: handle pre-gen + post-gen here
  template <typename HeightType>
  class CourseStitcher {
   public:
    CourseStitcher(
      const std::shared_ptr<HeightType>& height,
      const std::shared_ptr<SeedGrower>& grower,
      const ChunkConfig& chunk_config,
      const GrowConfig& grow_config,
      const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& pre_gen
    ) : hole_sampler(std::make_shared<SeedBasedHoleCreator<HeightType>>(
      height,
      grower,
      chunk_config,
      grow_config,
      pre_gen
    )), height(height), sampler_wrap(height), pre_gen(pre_gen) {
      auto post_gen_height =
        std::make_shared<terrain::PostGenHeightmap<HeightType>>(height, pre_gen, hole_sampler);

      post_gen = std::make_shared<mgc_course::mgc_gen::WorldFeatureManager>(
        post_gen_height
      );
    }

    typedef typename SeedBasedHoleCreator<HeightType>::sampler_type box_sampler_type;
    typedef impl::CourseSubSampler<HeightType> sub_sampler_type;

    float SampleHeight(double x, double y) const {
      return GetSubSampler(glm::dvec2(x, y))->Sample(x, y);
    }

    glm::vec4 SampleSplat(double x, double y, size_t index) const {
      return hole_sampler->SampleSplat(x, y, index);
    }

    float SampleTreeFill(double x, double y) const {
      // idk what the point of this was hehe
      return hole_sampler->SampleTreeFill(x, y);
    }

    float SampleGrassFill(double x, double y) const {
      return GetSubSampler(glm::dvec2(x, y))->SampleGrassFill(x, y);
    }

    std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager> GetPreGen() const {
      return pre_gen;
    }

    std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager> GetPostGen() const {
      return post_gen;
    }

    size_t WriteHeight(
      const glm::dvec2& origin,
      const glm::ivec2& sample_dims,
      double scale,
      float* output,
      size_t n_bytes
    ) {

      if (n_bytes < (sample_dims.x * sample_dims.y * sizeof(float))) {
        return 0;
      }

      // can prob arena alloc this
      float* temp = new float[sample_dims.x * sample_dims.y];

      sampler_wrap->WriteChunk(
        origin, sample_dims, scale, temp, n_bytes
      );

      cg::DataSampler<float> ds(sample_dims, temp);

      size_t res = hole_sampler->WriteHeight(
        origin,
        sample_dims,
        scale,
        ds,
        output,
        n_bytes
      );

      delete[] temp;
      return res;
    }

    size_t WriteSplat(
      const glm::dvec2& origin,
      const glm::ivec2& sample_dims,
      double scale,
      size_t index,
      glm::vec4* output,
      size_t n_bytes
    ) {
      return hole_sampler->WriteSplat(
        origin,
        sample_dims,
        scale,
        index,
        output,
        n_bytes
      );
    }

    size_t WriteTreeFill(
      const glm::dvec2& origin,
      const glm::ivec2& sample_dims,
      double scale,
      float* output,
      size_t n_bytes
    ) {
      return hole_sampler->WriteTreeFill(
        origin, sample_dims, scale, output, n_bytes
      );
    }

    std::shared_ptr<sub_sampler_type> GetSubSampler(const glm::dvec2& point) const {
      return GetSubSampler(point - glm::dvec2(0.5), glm::dvec2(1.0));

    }

    std::shared_ptr<sub_sampler_type> GetSubSampler(const glm::dvec2& origin, const glm::dvec2& size) const {
      auto box = hole_sampler->GetHoleSampler(origin, size);
      auto pre_gen_box = pre_gen->Prepare(origin, size);
      auto post_gen_box = post_gen->Prepare(origin, size);
      return std::make_shared<sub_sampler_type>(std::move(box), height, pre_gen_box, post_gen_box);
    }

    std::shared_ptr<sub_sampler_type> GetSubSampler(const chunker::ChunkIdentifier& id, double padding) {
      glm::dvec2 origin(id.x, id.y);
      origin -= padding;
      glm::dvec2 size(id.sample_dims.x * id.scale.AsDouble(), id.sample_dims.y * id.scale.AsDouble());
      size += 2 * padding;

      return GetSubSampler(origin, size);
    }

    std::vector<HoleInfo> GetHoleInfo(const glm::dvec2& origin, const glm::dvec2& size) const {
      return hole_sampler->GetHoleInfo(origin, size);
    }

   private:
    std::shared_ptr<SeedBasedHoleCreator<HeightType>> hole_sampler;
    std::shared_ptr<HeightType> height;
    std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager> pre_gen;
    std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager> post_gen;

    cg::SampleWriterGenericImpl<float, HeightType> sampler_wrap;
  };
}

#endif // COURSE_STITCHER_H_
