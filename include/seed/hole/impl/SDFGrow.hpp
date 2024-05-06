#ifndef MGC_SDF_GROW_H_
#define MGC_SDF_GROW_H_

#include <memory>
namespace mgc {
  template <typename SDF>
  class SDFGrow {
   public:
    SDFGrow(const std::shared_ptr<SDF>& sdf, double size) : sdf(sdf), grow(size) {};

    double Sample(double x, double y) const {
      return sdf->Sample(x, y) - grow;
    }

    double Sample(double x, double y, double z) const {
      return sdf->Sample(x, y, z) - grow;
    }

   private:
    std::shared_ptr<SDF> sdf;
    const double grow;
  };
}

#endif // MGC_SDF_GROW_H_
