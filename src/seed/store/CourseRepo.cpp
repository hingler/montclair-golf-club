#include "seed/hole/store/CourseRepo.hpp"

#include <vector>

namespace mgc {
  std::shared_ptr<const HoleBox> CourseRepo::ConvertPath(const std::vector<glm::dvec2>& points) {
    // default value for now
    // seed???
    return store.InsertBox<HoleBox>(points, 40.0, 512);
  }

  bool CourseRepo::Test(const glm::dvec2& point) const {
    std::unordered_set<std::shared_ptr<const HoleBox>> out;
    store.FetchPoint(point, out);
    // do a test on the contents of each point (eventually)
    for (auto& box : out) {
      if (box->TestPadding(point)) {
        return true;
      }
    }

    return false;
  }
}
