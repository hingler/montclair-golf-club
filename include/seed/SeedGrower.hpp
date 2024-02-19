#ifndef SEED_GROWER_H_
#define SEED_GROWER_H_

#include "seed/GrowConfig.hpp"
#include "seed/direct/DirectorBase.hpp"

#include "seed/path/SeedPath.hpp"
#include "seed/path/SeedPathBuilder.hpp"

#include <memory>
#include <vector>

#include <random>

namespace mgc {
  /// eyup
  class SeedGrower {
   public:
    SeedGrower();

    template <typename DirectorType, class... Args>
    void AddDirector(Args... args) {
      directors.push_back(std::make_unique<DirectorType>(args...));
    }

    /**
     * @brief Generates a path
     * 
     * @param init_point - start point
     * @param config - configuration for this generations
     * @return std::vector<std::shared_ptr<SeedPath>> 
     */
    std::vector<std::shared_ptr<SeedPath>> GeneratePaths(const glm::dvec2& init_point, const GrowConfig& config, std::seed_seq& sseq);

    // generate func - receive a config and return an array of paths
    // - delegate function: pass rigidity, branch dist, and energy
    // - run some time, make a branch, and then recur

    // course gen
    // - just step along and try to make a hole, greedily
    // - looking at real courses: "branches" tend to be longer, and split less
    // - (handle course gen appropriately)

    // how do we ensure course variety?
    // - a. generate as long as we can, and trim nodes down
    // - b. build "node chains" as we go, fill them in once we reach a termination point
    // - c. pre-determine a "yardage" and generate a course which matches it
    //   - if we're short: try to fill it in
    // - d. sliding yardage - get a stretch of "path length" to fill in with holes
    //   - like this the most - seems more resilient
    //   - eff: we go step by step, and append bits to our final yardage
    //   - once we have a sub-path: try to fill in some holes on that
    //   - start point is first safe point, end point is last safe point (or term)
    //   - yeah i like this : )
    // - e. try to generate courses within path nodes
    //   - each node represents one "hole"
    //   - instead of generating w a prespec'd branch length, we generate w a given yardage
    //     - on complete: we either branch, or we don't
    //     - if we run out of energy, just terminate early
    //     - instead of a branching dist, use a branching probability (lower for less hospitable terrain)

    //   - for course gen: leave some buffer space on the initial end (configurable i think)
    //     - when we go to generate the course: generate dynamically based on surrounding territory, and cut short if we have to

    //     - generate by total dist, not by dist traveled?

    //   - alt: will the courses be a bit too rigid? (ie: no dog legs???)

    //     - i think we'll be fine
    //     - i think: could introduce a turbulent director to push thingsd around a bit


    // wander logic
    // - courses tend to twist a bit more
    // - avoiding self-intersection when generating paths? eh
    // - i think just go with longer branch dists, larger angles

    // return gradient as sampled/weighted from components. mag >= 1.0.
    glm::dvec2 SampleDirection(const DirectorInfo& info) const;

    // returns gradient after accounting for configuration rigidity. normalized.
    glm::dvec2 SampleDirection_WithRigidity(const DirectorInfo& info, const GrowConfig& config) const;


   private:
    // list of dirs to fetch from
    std::vector<std::unique_ptr<DirectorBase>> directors;

    // engine which powers any random action
    std::mt19937_64 engine;


    /// @brief Recursive path generator
    /// @param builder - builder for this path
    /// @param init_point - point it is starting at
    /// @param init_direction - direction it is pointing in
    /// @param init_energy - initial energy supplied to path
    /// @param global_config - global config
    void GeneratePaths_recurse(
      SeedPathBuilder& builder,
      const glm::dvec2& init_point,
      const glm::dvec2& init_direction,
      double init_energy,
      const GrowConfig& global_config
    );


    std::uniform_real_distribution<double> len_dist = std::uniform_real_distribution(150.5, 675.0);
  };
}

// ex: dealing with water levels?
// - if we do something ewquiv: water level will be scripted in
// - ...and we'll add some component telling courses to "avoid water" or treat it as a "height floor" when generating terrain

// course gen
// - rationale for SDFs is that bounding boxes are pretty simple to calculate, and determinate
// - math should also (theoretically) be simpler
//   - (capsules will be a PITA for point lists :[)


#endif // SEED_GROWER_H_