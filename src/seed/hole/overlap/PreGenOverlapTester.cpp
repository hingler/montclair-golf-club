#include "seed/hole/overlap/PreGenOverlapTester.hpp"

namespace mgc {
  PreGenOverlapTester::PreGenOverlapTester(
    const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& manager,
    double grow_dist
  ) : manager(manager), grow_dist(grow_dist) {}

  bool PreGenOverlapTester::Test(const glm::dvec2& point) const {
    double sdf = this->manager->SampleOcclusion(point.x, point.y);
    return (sdf <= grow_dist);
  }
}
