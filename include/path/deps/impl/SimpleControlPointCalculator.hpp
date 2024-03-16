#ifndef SIMPLE_CONTROL_POINT_CALCULATOR_H_
#define SIMPLE_CONTROL_POINT_CALCULATOR_H_

#include "path/deps/ControlPointCalculator.hpp"

#include <random>

namespace mgc {
  namespace deps {
    namespace impl {
      class SimpleControlPointCalculator : public ControlPointCalculator {
        // if we need heightmaps down the line: provide in ctor :)
        // addl: return empty vector to signify a null?
        //
        // tba: pass engine instead of seed - pass engine
        // - engine owned by some root builder component
        // - pass into course bundle builder
        // - pass into sdf builders
       public:
        std::vector<size_t> GetControlPoints(
          const std::vector<glm::dvec2>& course_path,
          std::mt19937_64& engine
        ) const override;

        size_t GetPar(double len_sum) const;
      };
    }
  }
}

#endif // SIMPLE_CONTROL_POINT_CALCULATOR_H_
