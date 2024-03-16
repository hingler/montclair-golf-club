#ifndef SDF_HOLE_BOX_H_
#define SDF_HOLE_BOX_H_

#include "corrugate/FeatureBox.hpp"

#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/box/SmoothingTerrainBox.hpp"


#include "seed/hole/impl/SDFThresholdSampler.hpp"
#include <corrugate/sampler/splat/SplatManager.hpp>

namespace mgc {
  // is there a better way to handle the "underlying sampler" problem?
  // - write a mini

  // what am i thinking?
  // - the only reason i "might" need to keep sdfs somewhere, is if we end up testing against local fill
  // - ...but I don't REALLY need a solution for that
  //
  // - how could we do it though, if we wanted to?
  // - ehh
  //
  // also: is there a good way to resolve the "underlying sampler" problem?
  // - pass as arg to sample (bad coupling)
  // - cache via function call (don't smooth otherwise?)
  // - pass as arg to some external conversion func? doesnt fix the store prob unfortunately
  // - call as some prep func - we're pretty much insulating args anyway so why not?
  // - makes more sense to me too - worst case we can re-cache
  class SDFHoleBox : public cg::FeatureBox {
   public:
    SDFHoleBox(const cg::FeatureBox& source) : cg::FeatureBox(source) {}
    virtual std::unique_ptr<cg::BaseSmoothingSamplerBox> Convert() const = 0;
    /// return distance to nearest feature in box
    virtual double Dist(const glm::dvec2& point) const = 0;

    virtual ~SDFHoleBox() {}
  };

  namespace _impl {
    struct ZeroSampler {
      double Sample(double x, double y) const {
        return 0.0;
      }
    };
  }

  template <typename FT, typename GT, typename ST>
  class SDFHoleBoxImpl : public SDFHoleBox {
   public:
    SDFHoleBoxImpl(
      const cg::FeatureBox& source,
      std::shared_ptr<FT> fairway,
      std::shared_ptr<GT> green,
      std::shared_ptr<ST> sand
    ) : SDFHoleBox(source), fairway(fairway), green(green), sand(sand) {}

    std::unique_ptr<cg::BaseSmoothingSamplerBox> Convert() const override {
      // eventuall: use holeterrain type to map sdf -> terrain
      auto height = std::make_shared<_impl::ZeroSampler>();
      auto fill = std::make_shared<_impl::ZeroSampler>();
      auto splat = std::make_shared<cg::SplatManager>();

      splat->BindSamplers(
        std::make_shared<SDFThresholdSampler<FT>>(fairway),
        std::make_shared<SDFThresholdSampler<GT>>(green),
        std::make_shared<SDFThresholdSampler<ST>>(sand),
        std::make_shared<_impl::ZeroSampler>(),
        0
      );

      // tba: return box lol hehe haha r

      // we don't have a splat map hehe
      // what to do?
      // - move splat map to corrugate since we're handling some "terrain logic" there wrt boxes
      // - i'm fine w this - opens up that functionality in a diff ctx
      return std::make_unique<cg::SmoothingTerrainBox>(
        *this,
        height,
        splat,
        fill,
        0.9
      );
    }

    double Dist(const glm::dvec2& point) const override {
      // return min of all distances

      // need to convert to local space

      glm::dvec2 eff_point = point - GetOrigin();
      return std::min({
        fairway->Sample(eff_point.x, eff_point.y),
        green->Sample(eff_point.x, eff_point.y),
        sand->Sample(eff_point.x, eff_point.y)
      });
    }

   private:
    std::shared_ptr<FT> fairway;
    std::shared_ptr<GT> green;
    std::shared_ptr<ST> sand;
  };
}

// how to do this?
// - wrap SDFs in a way that makes them trivial to convert into samplers
// - idea: just come up with a templated spec that can create shit given any SDF

#endif // SDF_HOLE_BOX_H_
