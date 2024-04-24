#ifndef MGC_TREE_FILL_WRAP_H_
#define MGC_TREE_FILL_WRAP_H_

#include "chunker/ChunkIdentifier.hpp"
#include "terrain/CourseStitcher.hpp"
namespace mgc {
  namespace impl {
    template <typename SamplerType>
    class TreeFillDelegate {
     public:
      TreeFillDelegate(
        const std::shared_ptr<SamplerType>& sampler
      ) : sampler(sampler) {}

      float Sample(double x, double y) {
        return sampler->SampleTreeFill(x, y);
      }

     private:
      std::shared_ptr<SamplerType> sampler;
    };

    template <typename HeightType>
    class TreeFillWrap {
     public:
      typedef typename CourseStitcher<HeightType>::sub_sampler_type prep_type;
      typedef TreeFillDelegate<prep_type> sub_type;

      TreeFillWrap(
        const std::shared_ptr<CourseStitcher<HeightType>>& stitch
      ) : stitch(stitch) {}

      float Sample(double x, double y) const {
        // same thing as grass - subsample delegate which performs the transformation we want
        return stitch->SampleTreeFill(x, y);
      }

      std::shared_ptr<sub_type> Prepare(const chunker::ChunkIdentifier& id) const {
        return std::make_shared<sub_type>(
          stitch->GetSubSampler(id, 16.0)
        );
      }

     private:
      std::shared_ptr<CourseStitcher<HeightType>> stitch;
    };
  }
}

#endif // MGC_TREE_FILL_WRAP_H_
