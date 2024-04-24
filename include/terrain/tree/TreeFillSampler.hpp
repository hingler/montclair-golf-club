#ifndef MGC_TREE_FILL_SAMPLER_H_
#define MGC_TREE_FILL_SAMPLER_H_

#include "sdf/CPPCapsule.hpp"
#include <algorithm>
#include <glm/glm.hpp>

#include <memory>

#define _TFS_TREE_START 28.0
#define _TFS_TREE_SMOOTH 48.0

namespace mgc {
  template <typename FT, typename GT, typename ST>
  class TreeFillSampler {
   public:
    TreeFillSampler(
      const std::shared_ptr<FT>& fairway,
      const std::shared_ptr<GT>& green,
      const std::shared_ptr<ST>& sand,
      const std::vector<glm::dvec2>& path
    ) : fairway(fairway), green(green), sand(sand), hole_path(path, 36.0) {}

    float Sample(double x, double y) {
      // bake hole data down to a capsule that we can sample against
      // (use a preset width)
      // use dist from sdf + path
      // gap of a couple dozen yards, then tick up to 1.0 (based on padding??)
      double dist = std::min(
        std::min(
          fairway->Sample(x, y),
          green->Sample(x, y)
        ), std::min(
          sand->Sample(x, y),
          hole_path.Sample(x, y)
        )
      );

      double fac = glm::smoothstep(0.0, _TFS_TREE_SMOOTH, dist - _TFS_TREE_START);

      return fac;
    }
   private:
    std::shared_ptr<FT> fairway;
    std::shared_ptr<GT> green;
    std::shared_ptr<ST> sand;

    CPPCapsule hole_path;
  };
}

#endif // MGC_TREE_FILL_SAMPLER_H_
