#ifndef SEED_HEIGHT_WRAP_H_
#define SEED_HEIGHT_WRAP_H_

#include "chunker/ChunkIdentifier.hpp"
#include "terrain/CourseStitcher.hpp"

#include <memory>

namespace mgc {
  namespace impl {
    template <typename HeightType>
    class SeedHeightWrap {
     public:
      SeedHeightWrap(
        const std::shared_ptr<CourseStitcher<HeightType>>& ptr
      ) : ptr(ptr) {}

      double Sample(double x, double y) const {
        return ptr->SampleHeight(x, y);
      }

      double Get(double x, double y) const {
        return ptr->SampleHeight(x, y);
      }

      std::shared_ptr<typename CourseStitcher<HeightType>::sub_sampler_type> Prepare(const chunker::ChunkIdentifier& id) const {
        auto neighbors = reinterpret_cast<const chunker::util::Fraction*>(&id.neighbors);
        double neighbor_max = 0.0;
        for (size_t i = 0; i < (sizeof(id) / sizeof(chunker::util::Fraction)); i++) {
          neighbor_max = std::max(neighbor_max, neighbors[i].AsDouble());
        }

        // double-back, for sampling corner
        glm::dvec2 origin(
          static_cast<double>(id.x) - neighbor_max * 2,
          static_cast<double>(id.y) - neighbor_max * 2
        );

        origin += static_cast<glm::dvec2>(id.sample_offset);

        // double-fwd, to more-than-cover ground
        glm::dvec2 size(
          id.sample_dims.x * id.scale.AsDouble() + neighbor_max * 4,
          id.sample_dims.y * id.scale.AsDouble() + neighbor_max * 4
        );

        return ptr->GetSubSampler(origin, size);
      }

     private:
      std::shared_ptr<CourseStitcher<HeightType>> ptr;
    };
  }
}

#endif // SEED_HEIGHT_WRAP_H_
