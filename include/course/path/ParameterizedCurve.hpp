#ifndef PARAMETERIZED_CURVE_H_
#define PARAMETERIZED_CURVE_H_

#include <glm/glm.hpp>

namespace course {
  namespace path {
    /**
     * @brief Simple class for parameterized curves
     */
    class ParameterizedCurve {
    public:
      /**
       * @brief Samples this curve at some time t
       * 
       * @param time - [0, 1] - point along curve
       * @return glm::vec2 - outputted position
       */
      virtual glm::vec2 Sample(double time) const = 0;

      /**
       * @return length of this curve
       */
      virtual double Length() const = 0;

      virtual ~ParameterizedCurve() {};
    };
  }
}

#endif // PARAMETERIZED_CURVE_H_