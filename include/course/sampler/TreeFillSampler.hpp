#ifndef TREE_FILL_SAMPLER_H_
#define TREE_FILL_SAMPLER_H_

#include "type/sampler_type.hpp"

#include "debug/Logger.hpp"

#include <algorithm>
#include <cmath>

// a bit sparse - more tree variety will help
// need to leave course path mostly-open. new feature sampler :3
// need to come up with a solution for tree coverage on more distant meshes
// trees should be taller - also, vary in height
// courses should start getting a bit skinnier
// - tba: look into how to get higher tree counts - feels like impact is too hi???
// - also: look into doing a better job at dynamic population

// - lod system is a must - trees are very inefficient right now!!
// - would like to keep generating trees far away

// the must-haves!
// - larger, denser trees
// - avoid pop-in - was gonna use chunk sys to fade out trees at a dist, might come up w a better soln for that
// - (also, for pop-in: we'll probably need separate multimeshes for each LOD - come up with a solution which relies on instance data??)

// - 2: speed up start-up!! (if we can: avoid the hang on start)
// - 3: get collider chunks working!!! we can have them be sparse, but we need probably four, and they need to gen async (give them to a separate component)

// collider chunk impl
// - current cell, plus three cells adjacent to that quadrant
// - generate on thread, and feed back into collider later (chunks are 1025x!!)
// - (could also shrink it down a bit lole)
// - (wait initially)

// (does godot require us to supervise the main thread??)
// - collider component checks in on job status every frame
// - ((also: probably want to generify that future pattern lole))
// - whatever it gives us, we should be able to plug in right away (will have to check)
// - alt2: each "collider update" is delegated to a wrapper which checks its corresponding future every frame
// - (modify chunker to have an async path??)
// - there's a race cond on concurrent tasks rn but i think we can have a toggle to avoid that
// - also: caching approach isn't very dependable atm (bc our iterator approach isn't great)
// - although, actually: with the async path we take it shouldn't be a problem, bc we're fetching (for splats at least) chunk by chunk
// - so yeah, write a wrapper in the chunker repo which does it (create array of tasks, fetch and return - don't wait around!)
// - (alt: do we have a locking solution for the case where multiple threads are handling the same info at once? prob don't need it atm, just a bit of overhead)

// (got the generalized chunk manager working with splats)
// (tba: generalized chunk manager for our colliders)


namespace course {
  namespace sampler {
    template <typename FeatureSampler>
    class TreeFillSampler {
      static_assert(type::sampler_type<FeatureSampler>::value);

     public:
      TreeFillSampler(FeatureSampler sampler, float upper_fill, float lower_fill) : sampler_(sampler), upper_fill_(upper_fill), lower_fill_(lower_fill) {}

      double Get(double x, double y) const {
        // high fill probability at upper fill, dwindle down to lower fill
        // sample is log - smooth curve from like a 0.6 to 0
        float feature_sample = log(sampler_.Sample(x, y));
        if (feature_sample > upper_fill_) {
          feature_sample = 0.0;
        } else {
          // 0.0 here would be v positive
          feature_sample = std::max((feature_sample - lower_fill_) / (upper_fill_ - lower_fill_), 0.0f);
        }

        // 1.0 -> 0.0 - will dwindle a bit faster than that
        float res = powf(feature_sample, 1.2f) * 0.45f;
        return 1.0f;
      }
     private:
      FeatureSampler sampler_;
      float upper_fill_;
      float lower_fill_;
    };
  }
}

#endif // TREE_FILL_SAMPLER_H_