#ifndef CONTROL_POINT_CALCULATOR_H_
#define CONTROL_POINT_CALCULATOR_H_

#include <vector>

#include <glm/glm.hpp>

namespace mgc {
  namespace deps {
    class ControlPointCalculator {
     public:
      virtual std::vector<glm::dvec2> GetControlPoints(
        const std::vector<glm::dvec2>& course_path,
        size_t seed
      ) const = 0;
    };
  }
}

#endif // CONTROL_POINT_CALCULATOR_H_
