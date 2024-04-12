#ifndef MGC_GRASS_FILL_SAMPLER_H_
#define MGC_GRASS_FILL_SAMPLER_H_

#include "glm/ext/vector_common.hpp"
#include <algorithm>
#include <memory>

namespace mgc {
  template <typename FT, typename GT, typename ST>
  class GrassFillSampler {
   public:
    GrassFillSampler(
      const std::shared_ptr<FT>& fairway,
      const std::shared_ptr<GT>& green,
      const std::shared_ptr<ST>& sand
    ) : fairway(fairway), green(green), sand(sand) {}

    float Sample(double x, double y) {
      float min_dist = fairway->Sample(x, y);
      min_dist = std::min(min_dist, static_cast<float>(green->Sample(x, y)));
      min_dist = std::min(min_dist, static_cast<float>(sand->Sample(x, y)));

      min_dist = glm::max(min_dist, 0.0f);


      return glm::smoothstep(0.95f, 16.0f, min_dist);
    }

   private:
    std::shared_ptr<FT> fairway;
    std::shared_ptr<GT> green;
    std::shared_ptr<ST> sand;
  };
}

#endif // MGC_GRASS_FILL_SAMPLER_H_
