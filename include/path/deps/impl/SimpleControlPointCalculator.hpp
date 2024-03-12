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
       public:
        std::vector<glm::dvec2> GetControlPoints(
          const std::vector<glm::dvec2>& course_path,
          size_t seed
        ) const override;

        size_t GetPar(double len_sum) const;
       private:
        mutable std::mt19937_64 engine;
      };
    }
  }
}

#endif // SIMPLE_CONTROL_POINT_CALCULATOR_H_
