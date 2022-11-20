#ifndef I_SAMPLER_H_
#define I_SAMPLER_H_

namespace course {
  namespace sampler {
    /**
     * @brief interface for sampling terrain information.
     * Samplers extend infinitely in all directions -- if a sampled point is outside
     * the range of any defined features, we will simply return 0.
     */
    template <typename DataType>
    class ISampler {
    public:
      /**
       * @brief Samples the intensity of this sampler at a given world coordinate.
       * 
       * @param x - x coordinate
       * @param y - y coordinate 
       * @return float intensity of this sampler at the specified space.
       */
      virtual DataType Sample(float x, float y) const = 0;
    };
  }
}

#endif // I_SAMPLER_H_