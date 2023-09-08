#ifndef GAUSSIAN_FAIRWAY_SAMPLER_H_
#define GAUSSIAN_FAIRWAY_SAMPLER_H_

#include "course/sampler/GaussianMetaballSampler.hpp"
#include "course/path/CoursePath.hpp"
#include "course/path/CompoundCurve.hpp"

#include "course/sampler/gaussian/GaussianConfig.hpp"

#include <random>

namespace course {
  namespace sampler {
    namespace gaussian {
      /**
       * @brief Creates metaball samples to cover the course fairway
       */
      class GaussianFairwaySampler : public GaussianMetaballSampler {
       public:
        /**
         * @brief Construct a new Gaussian Fairway Sampler object
         * 
         * @param underlying_sampler - underlying sampler
         */
        GaussianFairwaySampler(
          const path::CoursePath& course_path,
          const path::CompoundCurve& bezier_curve
        );

        void Generate(const GaussianPathConfig& config);
       private:

        /**
         * @brief Creates fairway patches on underlying sampler
         * 
         * @param course_path - contains necessary patches to fill fairway cells
         * @param bezier_curve - bezier curve covering course path
         * @param density - density of metaballs on course
         * @param mean_sigma - avg sigma for metaballs placed along course path
         * @param intensity - multiplier for metaball intensity
         */
        void CreateFairwayNodes(
          const path::CoursePath& course_path,
          const path::CompoundCurve& bezier_curve,
          int density,
          double mean_sigma,
          double intensity
        );

        // node config
        // - intensity/sigma
        // - probability of filling tee (can handle w params trivially)
        // (tba: handle this inheritance better :3)

        /**
         * @brief Fills in path along fairway
         * 
         * @param course_path course node positions
         * @param bezier_curve path taken along course
         * @param density density of points along fairway path
         * @param mean_sigma sigma for points on path
         * @param intensity multiplier for fairway path intensity
         */
        void CreateFairwayPath(
          const path::CoursePath& course_path,
          const path::CompoundCurve& bezier_curve,
          int density,
          double mean_sigma,
          double intensity
          // might want to collate all of this config stuff into some builder instead of leaving it out
          // thinking we might want to separate this into its own implementation?? whatever
        );

        void CreateFairwayScatter(
          const path::CompoundCurve& bezier_curve,
          const GaussianFairwayScatterConfig& config
        );

        void CreateGreenNode(
          const glm::vec2& green_point,
          const GaussianGreenConfig& config
        );

        // (tba: there's no reason for this to be wrapped up in a class lole)

        

        // path config
        // - intensity/sigma
        // - density
        // - probability of joining tee to course
        // - probability of joining other course nodes
        // - scatter node behavior
        //    - whether or not to do it at all
        //    - scatter distance
        //    - scatter intensity/sigma
        //    - scatter density
        // separate scatter into its own call? (alt: we want scatter and join prob to be the same value)
        // thinking: we just need to pluck a couple of curves from the path stochastically and we get the same behavior
        // what's the best way to handle that??

        // - wrap rand gen and pass in??
        // - do something stupider?

       private:
        path::CoursePath path;
        path::CompoundCurve curve;
        std::mt19937_64 engine;
      };
    }
  }
}

#endif // GAUSSIAN_FAIRWAY_SAMPLER_H_