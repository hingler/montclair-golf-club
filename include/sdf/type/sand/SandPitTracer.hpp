#ifndef SAND_PIT_TRACER_H_
#define SAND_PIT_TRACER_H_

#include "glm/common.hpp"
#include "sdf/BasicTracer.hpp"
#include "sdf/InverseSDF.hpp"

#include <memory>

namespace mgc {
  namespace sand {
    // carves pit out
    template <typename BaseType>
    class SandPitTracer {
     public:
      SandPitTracer(
        const std::shared_ptr<BaseType>& sand_sdf,
        double pit_depth_scale
      ) : SandPitTracer(
        sand_sdf,
        std::make_unique<BasicTracerImpl<InverseSDF<BaseType>>>(std::make_shared<InverseSDF<BaseType>>(sand_sdf)),
        pit_depth_scale
      ) {}

      double Sample(double x, double y) const {
        double sample_2d = sand->Sample(x, y);
        if (sample_2d > 0.0) {
          return 0.0;
        }

        glm::dvec3 point(x, y, 0.0);
        glm::dvec3 direction(0.0, 0.0, -1.0);

        // smoothstep from 0.0 to (dist) based on init sample

        // works! but need to smooth out the edges :/

        TraceResult result = tracer->Trace(point, direction);

        double smoothing = glm::clamp(glm::smoothstep(0.0, 1.0, -sample_2d / 16.0), 0.0, 1.0);

        if (result.hit) {
          return -result.travel_dist * depth_scale * smoothing;
        }

        return 0.0;
      }
     private:
      SandPitTracer(
        const std::shared_ptr<BaseType>& base,
        std::unique_ptr<BasicTracer>&& tracer,
        double pit_depth_scale
      ) : sand(base), tracer(std::move(tracer)), depth_scale(pit_depth_scale) {}

      std::shared_ptr<BaseType> sand;
      std::unique_ptr<BasicTracer> tracer;
      const double depth_scale;
    };
  }
}

#endif // SAND_PIT_TRACER_H_
