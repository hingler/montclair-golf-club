#include "debug/draw/DebugDraw.hpp"
#include "debug/draw/SeedPathDraw.hpp"

#include "debug/draw/GradientDraw.hpp"

#include "seed/direct/impl/SimplexHeightFunc.hpp"
#include "seed/direct/GradNormalDirector.hpp"
#include "seed/SeedGrower.hpp"

#include "gog43/Logger.hpp"
#include "gog43/IOStreamLogger.hpp"

using namespace mgc;
using namespace _impl;

int main(int argc, char** argv) {
  gog43::provide_logger(new gog43::IOStreamLogger());
  auto draw = DebugDraw();

  std::shared_ptr<SimplexHeightFunc> func = std::make_shared<SimplexHeightFunc>(2, 4.0, 1024.0);

  auto func_draw = std::make_shared<GradientDraw<SimplexHeightFunc>>(func, 4.0, 8.0);
  draw.AddSampler(func_draw, glm::vec4(1.0, 1.0, 1.0, 1.0));

  SeedGrower grower;
  grower.AddDirector<GradNormalDirector<SimplexHeightFunc>>(func, 1.0);

  GrowConfig config;
  config.base_energy = 3125;
  config.branch_angle_degs = 65;
  config.rigidity = 0.99;
  config.step_size = 4.5;
  config.branch_probability = 0.6;

  std::seed_seq sseq(std::initializer_list<int>{142857, 1331, 2442});

  // path gen works, mostly!
  // what next?
  // - step along path sequences and make course
  // - want to re-visit course gen
  // - bezier path but a gaussian
  // - sand still the same
  // handling course gen
  // - iterate along a path node until we branch?
  //   - or: just get the path node and iterate along it
  // - the goal is to get sufficiently distant from surrounding courses, then make our own
  
  // course building
  // - courses fit into feature boxes
  // - when building a hole: make sure its far enough from all others
  // continue marching step by step until we hit some threshold yardage, or we encounter a course
  // take the snippet of resultant points and put a hole on it
  // - tee box
  // - fairway (paths, or patches)
  // - green
  // - sand patches
  // box the whole thing in
  // bounds: we can guess metaball bounds based on thresholds??
  // alt: use some conservative bounds-estimate
  // alt2: swap from metaballs to sdfs, and calculate the distance to our feature
  // - smin - i think sdfs would be better overall
  // - thinking: how will smin handle overlap??

  // write a little "sdf" component

  // lol i kinda like the idea of writing the sdf workload in rust as its own lib
  // - caller gets a "struct" representing the sdf
  // - functional: call funcs to add parts (lines, spheres, etc.)
  // - functional sample - sample the struct and return!!!
  // - lolwhynot :3
  auto path = grower.GeneratePaths(glm::dvec2(0, 0), config, sseq);

  if (path.size() > 0) {
    gog43::print("preparing to write image...");
    auto ptr = path[0];
    auto s = std::make_shared<SeedPathDraw>(ptr, 12.0);
    auto s_two = std::make_shared<SeedPathDraw>(path[1], 12.0);
    draw.AddSampler(s, glm::dvec4(1.0, 0.0, 0.0, 0.5));
    draw.AddSampler(s_two, glm::dvec4(0.0, 1.0, 0.0, 0.5));
    draw.WriteImage(glm::dvec2(-2048, -2048), glm::ivec2(1024, 1024), 4.0, "output.jpg");
  } else {
    gog43::print("didn't generate any paths idk why");
  }
}