#ifndef COMPOUND_CURVE_H_
#define COMPOUND_CURVE_H_

#include "course/path/ParameterizedCurve.hpp"
#include "course/path/BezierCurve.hpp"

#include <memory>
#include <vector>

namespace course {
  namespace path {
    class CompoundCurve : public ParameterizedCurve {
    public:
      CompoundCurve();
      glm::vec2 Sample(double time) const override;
      glm::vec2 Tangent(double time) const;
      double Length() const override;
      void Translate(const glm::vec2& offset) override;
      Rect GetBoundingBox() const override;

      void AddSegment(std::shared_ptr<BezierCurve> segment);
      void RemoveSegment(size_t index);

      /**
       * @param segment the segment whose end time we want
       * @return double - t where sample(t) = endpoint of specified segment or 0.0/1.0 if oob
       */
      double GetTimeForEndOfSpecifiedSegment(int segment) const;
    private:
      std::vector<std::shared_ptr<BezierCurve>> segments;
    };
  }
}

#endif // COMPOUND_CURVE_H_