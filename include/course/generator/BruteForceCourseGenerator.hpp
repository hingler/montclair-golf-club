#ifndef BRUTE_FORCE_COURSE_GENERATOR_H_
#define BRUTE_FORCE_COURSE_GENERATOR_H_

#include "course/generator/ICourseGenerator.hpp"

#include "course/generator/impl/seed_tracker_impl.hpp"
#include "course/generator/impl/PositionTracker.hpp"
#include "course/path/CoursePath.hpp"

#include <memory>
#include <random>

namespace course {
  namespace generator {
    class BruteForceCourseGenerator : public ICourseGenerator {
    public:
      BruteForceCourseGenerator();
      path::CoursePath GenerateCourse(std::shared_ptr<sampler::ISampler<float>> terrain_in, const glm::vec2& terrain_size_in);
      
      // input seed
      uint64_t seed;

      // length of this hoel
      float yardage;
    private:
      // create a random to accumulate seed state on startup
      std::mt19937_64 engine;
      std::shared_ptr<sampler::ISampler<float>> terrain;
      glm::vec2 terrain_size;
      glm::vec2 tee_point;

      std::uniform_real_distribution<double> simple_dist;
      std::uniform_int_distribution<unsigned int> binary_dist;

      glm::vec2 GenerateTeeLocation(std::shared_ptr<sampler::ISampler<float>> terrain, const glm::vec2& terrain_size);

      /**
       * @brief Generates a path from a given tee location.
       * @return path::fairway_point - a linked list of terrain points which map out the course.
       */
      impl::PositionTracker GenerateFromTee();

      

      /**
       * @brief given an initial point, outputs a seed associated with an initial point.
       * 
       * @param current_point - initial point
       * @param previous_point - previous shot - used to prune generation
       * @param shots_remaining - number of patches after this one which need to be generated
       * @param sample_depth - bit depth of number of samples taken from this point
       * @param impl::seed_tracker_uint64 - output var; contains seed identifying this path
       * @return score indicating the "quality" of this point (for now: negative means dump it)
       */
      float GenerateFromPoint(
        const glm::vec2& current_point,
        const glm::vec2& previous_point,

        int shots_remaining,

        int sample_depth,
        impl::PositionTracker& output
      );
    };
  }
}

#endif