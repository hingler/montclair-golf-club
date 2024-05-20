#ifndef MGC_HOLE_INFO_FETCHER_H_
#define MGC_HOLE_INFO_FETCHER_H_

#include "mgc/CourseGen_Multi.hpp"
namespace mgc {
  class HoleInfoFetcher {
   public:
    virtual std::vector<HoleInfo> GetHoleInfo(
      const glm::dvec2& origin, const glm::dvec2& size
    ) = 0;
  };

  template <typename HeightType>
  class HoleInfoFetcherImpl : public HoleInfoFetcher {
   public:
    HoleInfoFetcherImpl(const std::shared_ptr<CourseGen_Multi<HeightType>>& ptr) {
      gen_ptr = ptr;
    }

    std::vector<HoleInfo> GetHoleInfo(
      const glm::dvec2& origin, const glm::dvec2& size
    ) {
      return gen_ptr->GetHoleInfo(origin, size);
    }
   private:
    std::shared_ptr<CourseGen_Multi<HeightType>> gen_ptr;
  };
}

#endif // MGC_HOLE_INFO_FETCHER_H_
