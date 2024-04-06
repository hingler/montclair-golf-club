#ifndef BASIC_TRACER_H_
#define BASIC_TRACER_H_

#include <glm/glm.hpp>
#include <memory>

#define TRACE_EPS 0.0001

namespace mgc {
  struct TraceResult {
    bool hit;
    glm::dvec3 collision_point;
    double travel_dist;
    double sdf_dist;
  };

  class BasicTracer {
   public:
    // performs ray trace operation on a provided SDF
    virtual TraceResult Trace(const glm::dvec3& point, const glm::dvec3& direction, size_t max_iter = 250) const = 0;
    virtual ~BasicTracer() {}
  };

  template <typename SDFType>
  class BasicTracerImpl : public BasicTracer {
   public:
    BasicTracerImpl(const std::shared_ptr<SDFType>& sdf) : sdf(sdf) {}
    BasicTracerImpl(std::shared_ptr<SDFType>&& sdf) : sdf(sdf) {}
    TraceResult Trace(const glm::dvec3& point, const glm::dvec3& direction, size_t max_iter) const override {
      TraceResult result;
      result.sdf_dist = 0.0;
      result.travel_dist = 0.0;
      result.hit = false;

      glm::dvec3 cur(point);
      glm::dvec3 dir_n = glm::normalize(direction);

      for (size_t i = 0; i < max_iter; i++) {
        double dist = sdf->Sample(cur.x, cur.y, cur.z);
        if (dist < TRACE_EPS) {
          result.hit = true;
          break;
        }

        cur += (dir_n * dist);

        result.travel_dist += dist;
        result.sdf_dist = dist;
      }

      result.collision_point = cur;
      result.sdf_dist = sdf->Sample(cur.x, cur.y, cur.z);

      return result;
    }
   private:
    std::shared_ptr<SDFType> sdf;
  };
}

#endif // BASIC_TRACER_H_
