#ifndef MGC_FEATURE_INSTANCE_SPAWNER_H_
#define MGC_FEATURE_INSTANCE_SPAWNER_H_

#include "course/feature/WorldFeatureInstancer.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "glm/fwd.hpp"
namespace mgc {
  namespace mgc_course {
    namespace mgc_gen {
      class FeatureInstanceSpawner {
       public:
        FeatureInstanceSpawner(
          const std::shared_ptr<WorldFeaturePositioner>& positioner,
          const std::shared_ptr<WorldFeatureInstancer>& instancer
        );

        void Update(
          const glm::dvec3& position,
          double max_dist
        );

        void Clear();

       private:
        std::shared_ptr<WorldFeaturePositioner> positioner;
        std::shared_ptr<WorldFeatureInstancer> instancer;
      };
    }
  }
}

#endif // MGC_FEATURE_INSTANCE_SPAWNER_H_
