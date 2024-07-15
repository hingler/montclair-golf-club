#ifndef WORLD_FEATURE_INSTANCER_H_
#define WORLD_FEATURE_INSTANCER_H_

#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeature.hpp"
#include <memory>
#include <vector>
namespace mgc {
  namespace mgc_course {
    class WorldFeatureInstancer {
     public:
      virtual void UpdateInstances(
        const std::vector<FeatureSpec>& features
      ) = 0;
    };
  }
}

#endif // WORLD_FEATURE_INSTANCER_H_
