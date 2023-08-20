#ifndef PARAMETERIZED_CURVE_H_
#define PARAMETERIZED_CURVE_H_

#include "course/path/Rect.hpp"

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

      /**
       * @brief Translates this curve by a given "offset" vector.
       * 
       * @param offset - desired offset.
       */
      virtual void Translate(const glm::vec2& offset) = 0;

      /**
       * @brief Get the Rect which bounds this curve
       * 
       * @return Rect - rect bounding this curve
       */
      virtual Rect GetBoundingBox() const = 0;

      virtual ~ParameterizedCurve() {};
    };
  }
}

#endif // PARAMETERIZED_CURVE_H_