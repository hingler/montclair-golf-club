#ifndef BEZIER_CURVE_H_
#define BEZIER_CURVE_H_

#include "course/path/ParameterizedCurve.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace course {
  namespace path {
    class BezierCurve : public ParameterizedCurve {
    public:
      /**
       * @brief Construct a new Bezier Curve object
       */
      BezierCurve(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
      glm::vec2 Sample(double time) const override;
      double Length() const override;
      void Translate(const glm::vec2& offset) override;
      Rect GetBoundingBox() const override;
    private:
      glm::vec2 p0;
      glm::vec2 p1;
      glm::vec2 p2;
      glm::vec2 p3;

      // samples underlying bezier curve
      glm::vec2 SampleBezier_(double t) const;
      // gets extrema of curve along inputted axis, or -1 if dne
      glm::vec2 GetExtrema(int axis) const;
      void ArcLengthParameterize_(int num_samples);
      double GetParameterizedTime_(double t_absolute) const;
      // i believe we calculate how far we travel on each go
      // and then the last point is our dist

      // to sample:
      // we read the last point
      // we divide our time param by the total length
      // then we step along until we exceed that
      // our time points are assumed to be equidistant in time
      // so we interpolate the proper time value from that
      std::vector<double> distance_points;
    };
  }
}

#endif // BEZIER_CURVE_H_