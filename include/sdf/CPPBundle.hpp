#ifndef CPP_BUNDLE_H_
#define CPP_BUNDLE_H_

#include <glm/glm.hpp>

#include <vector>

#include "sdf/CPPCapsule.hpp"

namespace mgc {
  class CPPBundle {
   public:
    CPPBundle();
    CPPBundle(double k);

    void AddCircle(double x, double y, double radius);
    bool AddCapsule(
      const std::vector<glm::dvec2>& elements,
      const std::vector<double>& radii
    );

    void AddCapsule(
      const std::vector<glm::dvec2>& elements,
      double radius
    );

    double Sample(double x, double y) const;

    double k = 0.0;

   private:
    double smin_f(
      double a,
      double b
    ) const;

    std::vector<glm::dvec3> circles;

    // raw, or ptrs?
    // (tba: write capsule logic in cpp)
    std::vector<CPPCapsule> capsules;

    // shortcuts
    // - see if we can save sqrt for the end
    // - quick-dump - if x/y delta > (min dist + max radius + max point dist) then skip
    // - make SDF threshold skinnier (ie: 20 yards of padding instead of 256)
    // tomorrow: write this :3
  };
}


#endif // CPP_BUNDLE_H_
