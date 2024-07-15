#ifndef SDF_HOLE_BOX_H_
#define SDF_HOLE_BOX_H_

#include "corrugate/FeatureBox.hpp"

#include "corrugate/box/BaseSmoothingSamplerBox.hpp"
#include "corrugate/box/LocalSmoothTerrainBox.hpp"
#include "corrugate/box/SimpleConstBox.hpp"
#include "corrugate/box/SmoothingTerrainBox.hpp"


#include "corrugate/sampler/smooth/LocalizedSmoother.hpp"
#include "course/feature/WorldFeature.hpp"
#include "path/CourseBundle.hpp"
#include "sdf/type/sand/SandPitTracer.hpp"
#include "seed/hole/HoleBox.hpp"
#include "seed/hole/impl/SDFGrow.hpp"
#include "seed/hole/impl/SDFThresholdSampler.hpp"
#include "terrain/grass/GrassFillSampler.hpp"
#include "terrain/tree/TreeFillSampler.hpp"

#include <corrugate/sampler/splat/SplatManager.hpp>
#include <vector>

// goals
// - speed up splat gen a lil
// - dynamic terrain res (ie generating larger splats)

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
    SDFHoleBox(const glm::dvec2& origin, const glm::dvec2& size) : cg::FeatureBox(origin, size) {}
    virtual std::unique_ptr<cg::BaseSmoothingSamplerBox> Convert() const = 0;
    /// return distance to nearest feature in box
    virtual double Dist(const glm::dvec2& point) const = 0;

    virtual const CourseBundle& GetBundle() const = 0;

    virtual ~SDFHoleBox() {}
  };

  namespace _impl {
    struct ZeroSampler {
      double Sample(double x, double y) const {
        return 0.0;
      }
    };
  }

  template <typename FT, typename GT, typename ST, typename RT, typename BT, typename SmoothT>
  class SDFHoleBoxImpl : public SDFHoleBox {
    typedef cg::smooth::LocalizedSmoother<BT, SmoothT> smoother_type;
   public:
    SDFHoleBoxImpl(
      const HoleBox& source,
      const std::shared_ptr<FT>& fairway,
      const std::shared_ptr<GT>& green,
      const std::shared_ptr<ST>& sand,
      const std::shared_ptr<RT>& rough,
      const std::shared_ptr<BT>& base_height,
      const std::shared_ptr<smoother_type>& smoother,
      const CourseBundle& bundle
    ) : SDFHoleBox(source),
        fairway(fairway),
        green(green),
        sand(sand),
        rough(rough),
        base(base_height),
        smoother(smoother),
        course_path(bundle.course_path),
        bundle(bundle)
    {}

    std::unique_ptr<cg::BaseSmoothingSamplerBox> Convert() const override {
      // eventuall: use holeterrain type to map sdf -> terrain
      // tba: need to pass a smoother!
      auto height = std::make_shared<sand::SandPitTracer<ST>>(
        sand,
        0.28
      );



      // tba: need to write this
      auto tree_fill = std::make_shared<TreeFillSampler<FT, GT, ST>>(fairway, green, sand, course_path);
      auto splat = std::make_shared<cg::SplatManager>();

      // empty space -> 0-sample (black)

      // replace this with something that will handle trouncing prios??
      splat->BindSamplers(
        std::make_shared<SDFThresholdSampler<FT>>(fairway),
        std::make_shared<SDFThresholdSampler<GT>>(green),
        std::make_shared<SDFThresholdSampler<ST>>(sand, (-4.0)),
        std::make_shared<SDFThresholdSampler<RT>>(rough),
        0
      );

      auto grass_fill = std::make_shared<GrassFillSampler<FT, GT, SDFGrow<ST>>>(
        fairway,
        green,
        std::make_shared<SDFGrow<ST>>(sand, -4.5)
      );

      auto box = new cg::LocalSmoothTerrainBox<BT, SmoothT>(
        *this,
        height,
        splat,
        tree_fill,
        grass_fill,
        smoother
      );

      return std::unique_ptr<cg::BaseSmoothingSamplerBox>(box);
    }

    double Dist(const glm::dvec2& point) const override {
      // return min of all distances

      // need to convert to local space

      glm::dvec2 eff_point = point - GetOrigin();
      return std::min({
        fairway->Sample(eff_point.x, eff_point.y),
        green->Sample(eff_point.x, eff_point.y),
        sand->Sample(eff_point.x, eff_point.y),
        rough->Sample(eff_point.x, eff_point.y)
      });
    }

    const CourseBundle& GetBundle() const override {
      return bundle;
    }

   private:
    std::shared_ptr<FT> fairway;
    std::shared_ptr<GT> green;
    std::shared_ptr<ST> sand;
    std::shared_ptr<RT> rough;
    std::shared_ptr<BT> base;
    std::shared_ptr<smoother_type> smoother;
    std::vector<glm::dvec2> course_path;

    CourseBundle bundle;
  };
}

// how to do this?
// - wrap SDFs in a way that makes them trivial to convert into samplers
// - idea: just come up with a templated spec that can create shit given any SDF

#endif // SDF_HOLE_BOX_H_
