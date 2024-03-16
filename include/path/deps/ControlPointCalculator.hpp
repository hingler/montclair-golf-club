#ifndef CONTROL_POINT_CALCULATOR_H_
#define CONTROL_POINT_CALCULATOR_H_

#include <random>
#include <vector>

#include <glm/glm.hpp>

namespace mgc {
  namespace deps {
    class ControlPointCalculator {
     public:
      // returns indices of control points
      virtual std::vector<size_t> GetControlPoints(
        const std::vector<glm::dvec2>& course_path,
        std::mt19937_64& engine
      ) const = 0;

      virtual ~ControlPointCalculator() {}
    };
  }
}

#endif // CONTROL_POINT_CALCULATOR_H_
