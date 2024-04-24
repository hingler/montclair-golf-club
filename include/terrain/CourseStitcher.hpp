#ifndef COURSE_STITCHER_H_
#define COURSE_STITCHER_H_

#include "chunker/ChunkIdentifier.hpp"
#include "corrugate/sampler/DataSampler.hpp"
#include "corrugate/sampler/SampleWriterGeneric.hpp"
#include "seed/ChunkConfig.hpp"
#include "seed/GrowConfig.hpp"
#include "seed/SeedBasedHoleCreator.hpp"
#include "seed/SeedGrower.hpp"
#include "terrain/impl/CourseSubSampler.hpp"
#include <memory>

namespace mgc {
  // handles the work involved in combining
  // seed behavior with some underlying terrain


  // managing chunk config?
  // managing grow config?

  //
  template <typename HeightType>
  class CourseStitcher {
   public:
    CourseStitcher(
      const std::shared_ptr<HeightType>& height,
      const std::shared_ptr<SeedGrower>& grower,
      const ChunkConfig& chunk_config,
      const GrowConfig& grow_config
    ) : hole_sampler(
      height,
      grower,
      chunk_config,
      grow_config
    ), height(height), sampler_wrap(height) {}

    typedef typename SeedBasedHoleCreator<HeightType>::sampler_type box_sampler_type;
    typedef impl::CourseSubSampler<HeightType> sub_sampler_type;

    float SampleHeight(double x, double y) const {
      double base = height->Sample(x, y);
      double course = hole_sampler.SampleHeight(x, y, base);

      return base + course;
    }

    glm::vec4 SampleSplat(double x, double y, size_t index) const {
      return hole_sampler.SampleSplat(x, y, index);
    }

    float SampleTreeFill(double x, double y) const {
      // idk what the point of this was hehe
      return hole_sampler.SampleTreeFill(x, y);
    }

    float SampleGrassFill(double x, double y) const {
      // this is what a "single-stream" solution looks like
      // - not fantastic but should be fine i think : )
      // - (plus we like dont ever use it)
      return GetSubSampler(glm::dvec2(x, y))->SampleGrassFill(x, y);
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

      size_t res = hole_sampler.WriteHeight(
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
      return hole_sampler.WriteSplat(
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
      return hole_sampler.WriteTreeFill(
        origin, sample_dims, scale, output, n_bytes
      );
    }

    std::shared_ptr<sub_sampler_type> GetSubSampler(const glm::dvec2& point) const {
      auto box = hole_sampler.GetHoleSampler(point);
      return std::make_shared<sub_sampler_type>(std::move(box), height);

    }

    std::shared_ptr<sub_sampler_type> GetSubSampler(const glm::dvec2& origin, const glm::dvec2& size) const {
      auto box = hole_sampler.GetHoleSampler(origin, size);
      return std::make_shared<sub_sampler_type>(std::move(box), height);
    }

    std::shared_ptr<sub_sampler_type> GetSubSampler(const chunker::ChunkIdentifier& id, double padding) {
      glm::dvec2 origin(id.x, id.y);
      origin -= padding;
      glm::dvec2 size(id.sample_dims.x * id.scale.AsDouble(), id.sample_dims.y * id.scale.AsDouble());
      size += 2 * padding;

      return GetSubSampler(origin, size);
    }

   private:
    SeedBasedHoleCreator<HeightType> hole_sampler;
    std::shared_ptr<HeightType> height;

    cg::SampleWriterGenericImpl<float, HeightType> sampler_wrap;
  };
}

#endif // COURSE_STITCHER_H_
