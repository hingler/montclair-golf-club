#ifndef MGC_CLUBHOUSE_POSITIONER_H_
#define MGC_CLUBHOUSE_POSITIONER_H_

#include "course/feature/FeatureSpec.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "seed/SeedPositioner.hpp"
namespace mgc {
  namespace mgc_course {
    namespace prefeature {
      class ClubhousePositioner : public WorldFeaturePositioner {
       public:
        ClubhousePositioner(const std::shared_ptr<seed::SeedPositioner>& positioner);

        virtual std::vector<FeatureSpec> GetPoints(
          const glm::dvec2& origin,
          const glm::dvec2& size
        ) const override;

       private:
        std::shared_ptr<seed::SeedPositioner> positioner;
      };
    }
  }
}

#endif // MGC_CLUBHOUSE_POSITIONER_H_
