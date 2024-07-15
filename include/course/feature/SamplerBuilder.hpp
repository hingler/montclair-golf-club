#ifndef MGC_SAMPLER_BUILDER_H_
#define MGC_SAMPLER_BUILDER_H_

#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureInstancer.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
namespace mgc {
  namespace mgc_course {
    class SamplerBuilder {
     public:
      virtual std::shared_ptr<WorldFeaturePositioner> GetPositioner() const = 0;
      virtual std::shared_ptr<WorldFeatureBuilder> GetBuilder() const = 0;
    };
  }
}

#endif // MGC_SAMPLER_BUILDER_H_
