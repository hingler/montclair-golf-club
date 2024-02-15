#ifndef CURL_NOISE_DIRECTOR_H_
#define CURL_NOISE_DIRECTOR_H_

#include "seed/direct/DirectorBase.hpp"
#include "seed/direct/GradNormalDirector.hpp"
#include "seed/direct/impl/SimplexHeightFunc.hpp"

namespace mgc {
  class CurlNoiseDirector : public GradNormalDirector<_impl::SimplexHeightFunc> {
   public:
    CurlNoiseDirector(size_t octaves, double weight) : GradNormalDirector(std::make_shared<_impl::SimplexHeightFunc>(octaves), weight) {}
  };
}

#endif // CURL_NOISE_DIRECTOR_H_