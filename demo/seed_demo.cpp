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
  config.branch_angle_degs = 90;
  config.rigidity = 0.99;
  config.step_size = 4.5;
  config.branch_probability = 0.2;

  std::seed_seq sseq(std::initializer_list<int>{142857, 1331, 2442});

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