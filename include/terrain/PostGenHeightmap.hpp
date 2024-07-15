#ifndef POST_GEN_HEIGHTMAP_H_
#define POST_GEN_HEIGHTMAP_H_

#include "course/feature/WorldFeatureBuilder.hpp"
#include "seed/SeedBasedHoleCreator.hpp"
#include "course/feature/gen/WorldFeatureManager.hpp"
#include <memory>

namespace mgc {
  namespace terrain {
    template <typename HeightType>
    class PostGenHeightmap {
     public:
      PostGenHeightmap(
        const std::shared_ptr<HeightType>& base_height,
        const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& pre_gen,
        std::shared_ptr<SeedBasedHoleCreator<HeightType>> course_height
      ) : base_height(base_height), pre_gen(pre_gen), course_height(course_height) {}

      // should prob retool to make use of course sub sampler or smth hehe
      double Sample(double x, double y) const {
        double base = base_height->Sample(x, y);
        double pregen = pre_gen->Sample(x, y, base);
        double course = course_height->SampleHeight(x, y, (base + pregen));

        return base + pregen + course;
      }

     private:
      std::shared_ptr<HeightType> base_height;
      std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager> pre_gen;
      std::shared_ptr<SeedBasedHoleCreator<HeightType>> course_height;
    };
  }
}

#endif // POST_GEN_HEIGHTMAP_H_
