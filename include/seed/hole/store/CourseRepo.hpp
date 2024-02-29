#ifndef COURSE_REPO_H_
#define COURSE_REPO_H_

#include "seed/hole/store/OverlapTester.hpp"

#include "seed/hole/HoleBox.hpp"
#include "seed/path/node/PathNode.hpp"
#include <corrugate/MultiSampler.hpp>

namespace mgc {
  class CourseRepo : public OverlapTester {
   public:
    // want to modify this - doesn't work
    // - sdf boxes for local chunk (one func)
    // - hole boxes for other chunks (another func)
    // goal is to have consistent behavior - don't use SDFs if they exist
    // - ergo: hole boxes should constitute a conservative estimates
    // (how to deal with it?)

    // dealing with bleed on chunks?? (ie: use boxes originating from chunks, or using boxes *in* chunks??)
    // - chunks crossing btwn margins will break
    // based on location will break invar on boundaries
    // - based on source: gen will disappear on boundaries (ie: "home chunk" is encountering "foreign chunk" at boundaries - neither will gen)
    // - nothing else we can do - will decouple (tba it)

    // ergo:
    // - our impl should distinguish holes and SDFs
    // - we also need to associate boxes with chunks for testing (will be done!)

    // - fetch hole boxes for sampled chunk
    // - fetch hole boxes for neighboring chunks
    // - put neighbor hole boxes into repo as holes
    //   - also: need SDFs for neighboring chunks lol
    //   - alt1: convert hole box -> hole SDF -> hole terrain based on proximity to sample point
    //   - if the hole box is a conservative estimate then we can just fetch from chunk write
    //   
    // - start generating hole boxes in sampled chunk - as we go, 

    // options
    // - wrap chunk seeds in a box - if we touch that box, THEN create SDFs for that chunk.
    //   - (conservative, I guess)
    //   - same rules in all cases - hole boxes are a conservative estimate, so avoid them
    // - SDF -> Sampler is super easy right now I think.

    // impl
    // - on sample, generate paths for 3x3 chunks 
    //   - (queue any chunks we're inside, run that until complete)
    //   - store the resultant paths somewhere, assc'd with their resp chunks
    //   - bounding box
    // - if we're inside a chunk's path box, then generate that chunk
    //   - load up seed paths for neighbors (should already be done)
    //   - store hole boxes (ie padded seed paths)
    //   - for each hole box (in random order):
    //     - test path (via gen) against neighbor hole boxes, and local sdf boxes
    //     - once a path is built, create an SDF box for it
    //     - add that box to our "tester"
    //   - once all paths have been visited, turn the resultant SDF boxes into samplers


    // gen
    // - we generate hole boxes in a 3x3 radius
    // 

    std::shared_ptr<const HoleBox> ConvertPath(const std::vector<glm::dvec2>& path);
    bool Test(const glm::dvec2& point) const override;
   private:
    cg::MultiSampler<HoleBox> store;
  };
}
#endif // COURSE_REPO_H_
