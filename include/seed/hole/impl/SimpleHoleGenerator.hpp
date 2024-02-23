#ifndef SIMPLE_HOLE_GENERATOR_H_
#define SIMPLE_HOLE_GENERATOR_H_

#include "seed/hole/HoleGenerator.hpp"

#include "seed/hole/impl/SimpleSDFHoleBuilder.hpp"
#include "seed/hole/store/OverlapTester.hpp"

#include "traits/mgc_sampler.hpp"

#include <memory>

namespace mgc {
  template <typename HeightType>
  class SimpleHoleGenerator : public HoleGenerator {
    static_assert(traits::sampler_type<HeightType>::value);

   public:
    SimpleHoleGenerator(std::shared_ptr<HeightType> height, std::shared_ptr<OverlapTester> tester)
    : terrain(height), tester(tester), delegate_builder(height) {}

    std::shared_ptr<SDFHoleBox> GenerateHole(const HoleBox& box) override {
      HoleBox::iterator itr = box.begin();
      while (box.TestPadding(*itr) && itr != box.end()) {
        ++itr;
      }

      std::vector<glm::dvec2> points;
      while (!box.TestPadding(*itr) && itr != box.end()) {
        points.push_back(*itr);
        ++itr;
      }

      // encountered an invalid point
      // check what we have so far, and see if we can make a course out of it
      if (points.size() > 0) {
        double total_dist = glm::length(points[points.size() - 1] - points[0]);
        if (total_dist > MIN_COURSE_LEN) {
          // call code to create a course path

        }
      }
      // if net dist is greater than some threshold, then try to build a course out of it
      // pass to another func to actually lay out the points?

      // attempt to return multiple? ehh later
      return std::make_shared<SDFHoleBox>(nullptr);
    }

    // i guess this is good : )
    SimpleHoleGenerator(const SimpleHoleGenerator &) = default;
    SimpleHoleGenerator(SimpleHoleGenerator &&) = default;
    SimpleHoleGenerator &operator=(const SimpleHoleGenerator &) = default;
    SimpleHoleGenerator &operator=(SimpleHoleGenerator &&) = default;

   private:
    const std::shared_ptr<HeightType> terrain;
    const std::shared_ptr<OverlapTester> tester;

    SimpleSDFHoleBuilder<HeightType> delegate_builder;

    static constexpr double MIN_COURSE_LEN = 116.0;
  };
}

#endif // SIMPLE_HOLE_GENERATOR_H_
