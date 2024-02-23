#ifndef SDF_BUNDLE_CPP_H_
#define SDF_BUNDLE_CPP_H_

#include <sdf_jak/sdf_bundle.h>

#include <glm/glm.hpp>

namespace mgc {
  class Bundle {
   public:
    /// create a new sdf bundle
    Bundle() : Bundle(0.0) {};
    Bundle(double k);

    void AddCircle(double x, double y, double radius);
    void AddCapsule(const glm::dvec2* elements, size_t count, double radius);
    double Sample(double x, double y);

    SDFBundle* raw_ptr() { return bundle; }

    ~Bundle();

    Bundle(const Bundle& other) = delete;
    Bundle& operator=(const Bundle& other) = delete;

    Bundle(Bundle&& other);
    Bundle& operator=(Bundle&& other);

    // thinking: i really like rust's approach to copy
   private:
    SDFBundle* bundle;
  };
}

#endif // SDF_BUNDLE_CPP_H_
