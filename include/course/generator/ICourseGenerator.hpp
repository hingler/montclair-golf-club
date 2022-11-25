#ifndef I_COURSE_GENERATOR_H_
#define I_COURSE_GENERATOR_H_

#include "course/sampler/ISampler.hpp"
#include "course/path/CoursePath.hpp"

#include <memory>

namespace course {
  namespace generator {
    class ICourseGenerator {
    public:
      // create a method which returns the head of a list of fairway points
      // we probably want to separate terrain scoring and heights

      // lets call it a todo
      virtual path::CoursePath GenerateCourse(
        std::shared_ptr<sampler::ISampler<float>> terrain_in,
        const glm::vec2& terrain_size_in
        // sample accuracy im not going to worry about yet
      ) = 0;


      // virtual void AssignSeed(uint64_t seed) = 0;

      virtual ~ICourseGenerator() {}
      

      // we'll manager the rest internally

      // i figure virtual is fine here, this method shouldn't be high throughput.
      // do we need it? probably not.
      // but i like that it's virtual :D
    };
  }
}

#endif