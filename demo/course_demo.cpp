#include "debug/draw/DebugDraw.hpp"
#include "debug/draw/GradientDraw.hpp"

#include "debug/draw/SeedCreatorDraw.hpp"
#include "seed/SeedBasedHoleCreator.hpp"

#include "gog43/Logger.hpp"
#include "gog43/IOStreamLogger.hpp"
#include "seed/direct/GradNormalDirector.hpp"
#include "seed/direct/impl/SimplexHeightFunc.hpp"

using namespace gog43;
using namespace mgc;

static glm::vec4 colors[4] = {
  glm::vec4(1, 0, 0, 1),
  glm::vec4(0, 1, 0, 1),
  glm::vec4(0, 0.4, 1, 1),
  glm::vec4(1, 1, 0, 1)
};

int main(int argc, char** argv) {
  provide_logger(new IOStreamLogger());
  auto draw = DebugDraw();

  auto height = std::make_shared<_impl::SimplexHeightFunc>(3, 8.0, 1024.0);
  auto func_draw = std::make_shared<GradientDraw<_impl::SimplexHeightFunc>>(height, 0.0, 4.0);

  auto grower = std::make_shared<SeedGrower>();
  grower->AddDirector<GradNormalDirector<_impl::SimplexHeightFunc>>(height, 1.0);

  GrowConfig config;
  config.base_energy = 3125;
  config.branch_angle_degs = 65;
  config.rigidity = 0.99;
  config.step_size = 4.5;
  config.branch_probability = 0.6;

  ChunkConfig chunk_config;
  chunk_config.chunk_size = 4096;
  chunk_config.seed_dist = 1536;
  chunk_config.seed_origin = glm::dvec2(0);

  auto c = std::make_shared<SeedBasedHoleCreator<_impl::SimplexHeightFunc>>(
    height,
    grower,
    chunk_config,
    config
  );

  for (int i = 0; i < 3; i++) {
    auto c_draw = std::make_shared<SeedCreatorDraw<_impl::SimplexHeightFunc>>(
      c,
      0,
      i
    );

    draw.AddSampler(c_draw, colors[i]);
  }

  // hi
  // shout out your favorite color in the chat

  // among us tasks
  // - add course spacing to config
  // - work on thickening course outlines
  //   - turbulent displace
  //   - include size on each points
  // - add sand traps (behavior for...)
  //   - courses should be able to be asymm? how to implement? (toss around a couple circles and smooth the whole thing)
  // - start adding more features (now that we have a piss simple builder to verify with)


  // thinking: would be good to break sdf gen into several parts working in conjunction
  // - 1. get a course path (acc some yardage)

  // next goal should be to improve generation appearance
  // - would like more erratic course trajectories, if we can
  //   - atm: would need to handle in path gen (introduce sharp breaks in generation)
  // - want to vary thickness
  //   - float corresp for dist - connect dist point to path spec
  // - obv: need a teebox, and a green

  // 60 kilometers drawn
  draw.WriteImage(glm::dvec2(-1024, -1560), glm::ivec2(2048, 2048), 12.0, "courses.jpg");

  // chunk draw
  // call sample - don't mind the cost
}
