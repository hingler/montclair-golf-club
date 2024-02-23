#ifndef COURSE_REPO_H_
#define COURSE_REPO_H_

#include "seed/hole/store/OverlapTester.hpp"

#include "seed/hole/HoleBox.hpp"
#include "seed/path/node/PathNode.hpp"
#include <corrugate/MultiSampler.hpp>

namespace mgc {
  class CourseRepo : public OverlapTester {
   public:
    std::shared_ptr<const HoleBox> ConvertPath(const std::vector<glm::dvec2>& path);
    bool Test(const glm::dvec2& point) const override;
   private:
    cg::MultiSampler<HoleBox> store;
  };
}
#endif // COURSE_REPO_H_
