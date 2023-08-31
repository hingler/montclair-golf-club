#ifndef GAUSSIAN_METABALL_SAMPLER_H_
#define GAUSSIAN_METABALL_SAMPLER_H_

#include <vector>

#include "course/sampler/impl/GaussianMetaball.hpp"

#include <glm/glm.hpp>

namespace course {
  namespace sampler {
    /**
     * @brief Metaball sampler which makes use of a gaussian function.
     *        Want to avoid ISampler interface bc it incurs a heavy cost over large sample ct
     */
    class GaussianMetaballSampler {
     public:
      /**
       * @brief Samples the underlying metaball sampler
       * 
       * @param x - x coord
       * @param y - y coord
       * @return double - the intensity of the metaball func at this point
       */
      double Sample(double x, double y) const;

      glm::vec2 Gradient(double x, double y) const;

      // TODO: return gradient at a point

      /**
       * @brief Adds a gaussian metaball to this sampler
       * 
       * @param x - coord
       * @param y - coord
       * @param sigma - stdev for gaussian function over dist from origin
       * @param intensity - multiplier applied to underlying gaussian
       */
      void Add(double x, double y, double sigma, double intensity);

      /**
       * @brief Add one sampler's contents to another.
       * 
       * @param other - other sampler
       * @param intensity_mod - modification made to other sampler intensity
       */
      void Merge(const GaussianMetaballSampler& other, double intensity_mod);
      
      /**
       * @brief Clears all stored samples.
       */
      void Clear();
     private:
      std::vector<impl::GaussianMetaball> metaballs_;
    };
  }
}

#endif // GAUSSIAN_METABALL_SAMPLER_H_