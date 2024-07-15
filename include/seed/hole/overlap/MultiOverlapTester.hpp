#ifndef MGC_MULTI_OVERLAP_TESTER_H_
#define MGC_MULTI_OVERLAP_TESTER_H_

#include "seed/hole/overlap/OverlapTester.hpp"
namespace mgc {
  class MultiOverlapTester : public OverlapTester {
   public:
    MultiOverlapTester(
      const std::vector<std::shared_ptr<OverlapTester>>& testers
    ) : testers(testers) {}

    bool Test(const glm::dvec2& point) const {
      for (const auto& tester : testers) {
        if (tester->Test(point)) {
          return true;
        }
      }

      return false;
    }
   private:
    std::vector<std::shared_ptr<OverlapTester>> testers;
  };
}

#endif // MGC_MULTI_OVERLAP_TESTER_H_
