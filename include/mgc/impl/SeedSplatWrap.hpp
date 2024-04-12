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

      // how to do this hehe
      size_t WriteSplat(
        const glm::dvec2& origin,
        const glm::ivec2& sample_dims,
        double scale,
        size_t index,
        glm::vec4* output,
        size_t n_bytes
      ) const {
        size_t res = ptr->WriteSplat(
          origin, sample_dims, scale, index, output, n_bytes
        );

        size_t elems = (res / sizeof(glm::vec4));
        for (size_t i = 0; i < elems; i++) {
          // want a default value
          if (glm::length(output[i]) < 0.01) {
            output[i] = glm::vec4(0, 0, 0, 1);
          }
        }

        return res;
      }

     private:
      std::shared_ptr<CourseStitcher<HeightType>> ptr;
    };
  }
}

#endif // SEED_SPLAT_WRAP_H_
