#ifndef MGC_GRASS_FILL_WRAP_H_
#define MGC_GRASS_FILL_WRAP_H_

#include "chunker/ChunkIdentifier.hpp"
#include "terrain/CourseStitcher.hpp"

namespace mgc {
  namespace impl {
    // del for sub sampler
    template <typename SamplerType>
    class GrassFillSubsampleDelegate {
     public:
      GrassFillSubsampleDelegate(
        const std::shared_ptr<SamplerType>& sampler
      ) : sampler(sampler) {}

      float Sample(double x, double y) {
        return sampler->SampleGrassFill(x, y);
      }
     private:
      std::shared_ptr<SamplerType> sampler;
    };


    template <typename HeightType>
    class GrassFillWrap {
     public:
      GrassFillWrap(
        const std::shared_ptr<CourseStitcher<HeightType>>& stitch
      ) : stitch(stitch), underlying(stitch) {}

      float Sample(double x, double y) const {
        return underlying.Sample(x, y);
      }

      typedef typename CourseStitcher<HeightType>::sub_sampler_type prep_type;
      typedef GrassFillSubsampleDelegate<prep_type> sub_type;

      std::shared_ptr<sub_type> Prepare(const chunker::ChunkIdentifier& id) const {
        glm::dvec2 origin(id.x, id.y);
        glm::dvec2 size(id.sample_dims.x * id.scale.AsDouble(), id.sample_dims.y * id.scale.AsDouble());

        auto subsampler = stitch->GetSubSampler(origin, size);
        return std::make_shared<sub_type>(
          subsampler
        );
      }
     private:
      std::shared_ptr<CourseStitcher<HeightType>> stitch;
      GrassFillSubsampleDelegate<CourseStitcher<HeightType>> underlying;
    };
  }
}

#endif // MGC_GRASS_FILL_WRAP_H_
