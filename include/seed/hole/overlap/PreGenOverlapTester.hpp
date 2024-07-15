#ifndef MGC_PRE_GEN_OVERLAP_TESTER_H_
#define MGC_PRE_GEN_OVERLAP_TESTER_H_

#include "course/feature/gen/WorldFeatureManager.hpp"
#include "seed/hole/overlap/OverlapTester.hpp"
namespace mgc {
  class PreGenOverlapTester : public OverlapTester {
   public:
    PreGenOverlapTester(
      const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& manager,
      double grow_dist
    );

    bool Test(const glm::dvec2& point) const override;

   private:
    std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager> manager;
    double grow_dist;
  };
}

#endif // MGC_PRE_GEN_OVERLAP_TESTER_H_
