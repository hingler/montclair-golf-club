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
      double Length() const override;

      void AddSegment(std::shared_ptr<BezierCurve> segment);
    private:
      std::vector<std::shared_ptr<BezierCurve>> segments;
    };
  }
}

#endif // COMPOUND_CURVE_H_