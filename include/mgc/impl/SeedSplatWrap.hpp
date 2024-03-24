#ifndef SEED_SPLAT_WRAP_H_
#define SEED_SPLAT_WRAP_H_

#include "terrain/CourseStitcher.hpp"
namespace mgc {
  namespace impl {
    template <typename HeightType>
    class SeedSplatWrap {
     public:
      SeedSplatWrap(
        const std::shared_ptr<CourseStitcher<HeightType>>& ptr
      ) : ptr(ptr) {}

      glm::vec4 Sample(double x, double y, size_t index) const {
        return ptr->SampleSplat(x, y, index);
      }

      size_t WriteSplat(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        size_t index,
        glm::vec4* output,
        size_t n_bytes
      ) const {
        return ptr->WriteSplat(
          origin, sample_dims, scale, index, output, n_bytes
        );
      }

     private:
      std::shared_ptr<CourseStitcher<HeightType>> ptr;
    };
  }
}

#endif // SEED_SPLAT_WRAP_H_
