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
       * @return the number of points tracked by this curve
       */
      virtual int GetControlPointCount() = 0;

      /**
       * @brief Gets a desired control point if it exists
       * 
       * @param index 
       * @return glm::vec2 
       */
      virtual glm::vec2 GetControlPoint(int index) = 0;

      /**
       * @brief Samples this curve at some time t
       * 
       * @param time - [0, 1] - point along curve
       * @return glm::vec2 - outputted position
       */
      virtual glm::vec2 Sample(double time) = 0;

      /**
       * @return length of this curve
       */
      virtual double Length() = 0;

      virtual ~ParameterizedCurve() {};
    };
  }
}

#endif // PARAMETERIZED_CURVE_H_