#ifndef SIMPLE_FAIRWAY_SAMPLER_H_
#define SIMPLE_FAIRWAY_SAMPLER_H_

// sampler which follows a spline and course path
// to produce fairway splat

#include "course/sampler/ISampler.hpp"

#include "course/path/CoursePath.hpp"
#include "course/path/ParameterizedCurve.hpp"
#include "course/path/CompoundCurve.hpp"
#include "course/sampler/MetaballSampler.hpp"

#include <random>

namespace course {
  namespace sampler {
    namespace grass {
      // thinking: generalize fairway sampler to a "metaball sampler"
      // also: avoid the green? (nvm - put the green on top of it)
      class SimpleGrassSampler : public ISampler<float> {
      public:
        SimpleGrassSampler(const path::CoursePath& course_path, const path::CompoundCurve& bezier_curve, uint64_t seed);
        float Sample(float x, float y) const override;

        void SetThresholdModifier(float threshold);
        float GetThresholdModifier();

        
      private:
        // fill in our course points
        void CreatePatches_(const path::CoursePath& course_path, const path::CompoundCurve& bezier_curve, int density);

        // fill in our path
        void FillPath_(const path::CoursePath& course_path, const path::CompoundCurve& bezier_curve);
        
        // fills in a range along our path
        void FillRange_(const path::CompoundCurve& bezier_curve, double min_t, double max_t, int density);
        std::mt19937_64 engine;
        MetaballSampler underlying_sampler;
      };
    }
  }
}

#endif // SIMPLE_FAIRWAY_SAMPLER_H_