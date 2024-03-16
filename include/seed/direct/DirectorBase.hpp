#ifndef DIRECTOR_BASE_H_
#define DIRECTOR_BASE_H_

#include <glm/glm.hpp>

#include "seed/direct/DirectorInfo.hpp"

namespace mgc {
  class DirectorBase {
   public:
    DirectorBase(double weight) : weight(weight) {}
    
    glm::dvec2 SampleDirection(const DirectorInfo& context) {
      return SampleDirection_impl(context) * weight;
    }

    virtual ~DirectorBase() {};
   private:
    // returns a vector indicating the next direction of travel
    virtual glm::dvec2 SampleDirection_impl(const DirectorInfo& context) const = 0;

    // weight applied to this director, wrt all other directors
    const double weight;
  };
}

#endif // DIRECTOR_BASE_H_