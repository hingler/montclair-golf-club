#ifndef MGC_HOLE_INFO_FETCHER_H_
#define MGC_HOLE_INFO_FETCHER_H_

#include "seed/hole/ConversionResult.hpp"
#include <vector>
#include <glm/glm.hpp>
namespace mgc {
  class HoleInfoFetcher {
   public:
    virtual std::vector<HoleInfo> GetHoleInfo(
      const glm::dvec2& origin, const glm::dvec2& size
    ) const = 0;
  };
}

#endif // MGC_HOLE_INFO_FETCHER_H_
