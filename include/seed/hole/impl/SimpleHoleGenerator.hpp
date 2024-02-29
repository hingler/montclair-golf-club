#ifndef SIMPLE_HOLE_GENERATOR_H_
#define SIMPLE_HOLE_GENERATOR_H_

#include "seed/hole/HoleGenerator.hpp"

#include "seed/hole/impl/SimpleSDFHoleBuilder.hpp"
#include "seed/hole/store/OverlapTester.hpp"

#include "traits/mgc_sampler.hpp"

#include <memory>

namespace mgc {
  // creates SDF holes from hole boxes
  template <typename HeightType>
  class SimpleHoleGenerator : public HoleGenerator {
    static_assert(traits::sampler_type<HeightType>::value);

   public:
    SimpleHoleGenerator(std::shared_ptr<HeightType> height, std::shared_ptr<OverlapTester> tester)
    : terrain(height), tester(tester), delegate_builder(height) {}

    std::unique_ptr<SDFHoleBox> GenerateHole(const HoleBox& box) override {
      HoleBox::iterator itr = box.begin();
      while (itr != box.end() && tester->Test(*itr)) {
        ++itr;
      }

      // point get
      std::vector<glm::dvec2> points;
      points.push_back(*itr);
      while (itr != box.end() && !tester->Test(*(++itr))) {
        points.push_back(*itr);
        ++itr;
      }

      double min_dist = glm::length(points[points.size() - 1] - points[0]);
      if (min_dist > MIN_COURSE_LEN) {
        return delegate_builder.CreateSDF(box, points, min_dist);
      }

      // couldn't create course
      return std::make_unique<SDFHoleBox>(nullptr);
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
