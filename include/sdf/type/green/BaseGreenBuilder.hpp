#ifndef BASE_GREEN_BUILDER_H_
#define BASE_GREEN_BUILDER_H_

#include "sdf/Bundle.hpp"
#include "sdf/CPPBundle.hpp"
#include "sdf/SDFBuilderType.hpp"

namespace mgc {
  namespace green {
    class BaseGreenBuilder : public SDFBuilderType<CPPBundle> {
     public:
      std::shared_ptr<CPPBundle> Get(
        const CourseBundle& bundle,
        std::mt19937_64& engine
      ) const;

     private:
      void GenerateGreenPatches(
        const glm::dvec2& origin,
        std::uniform_real_distribution<double>& offset_xy,
        std::normal_distribution<double>& radius,
        size_t patch_count,
        std::mt19937_64& engine,
        const std::shared_ptr<CPPBundle>& output
      ) const;
    };
  }
}

#endif // BASE_GREEN_BUILDER_H_
