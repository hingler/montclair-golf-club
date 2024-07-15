#ifndef UNDERLYING_SAMPLER_HEIGHT_H_
#define UNDERLYING_SAMPLER_HEIGHT_H_

#include "corrugate/sampler/SampleWriterGeneric.hpp"
#include "course/feature/gen/WorldFeatureManager.hpp"
namespace mgc {
  namespace mgc_course {
    class UnderlyingSamplerHeight {
     public:
      UnderlyingSamplerHeight(
        const std::shared_ptr<cg::SampleWriterGeneric<double>>& base_height,
        const std::shared_ptr<mgc_gen::WorldFeatureManager>& underlying
      ) : base_height(
        base_height
      ), underlying(underlying) {}

      double Sample(double x, double y) const {
        double base = base_height->Sample(x, y);
        return base + underlying->Sample(x, y, base);
      }
     private:
      std::shared_ptr<cg::SampleWriterGeneric<double>> base_height;
      std::shared_ptr<mgc_gen::WorldFeatureManager> underlying;
    };
  }
}

#endif // UNDERLYING_SAMPLER_HEIGHT_H_
