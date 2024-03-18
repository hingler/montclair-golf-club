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
  config.base_energy = 2841;
  config.branch_angle_degs = 65;
  config.rigidity = 0.98;
  config.step_size = 4.6;
  config.branch_probability = 0.5;

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

  for (int i = 0; i < 4; i++) {
    // indiv samples are about 30us
    // - 35 / ms (not great)
    // - 35k / sec (not great either)
    // bulk of the time is spent on sampling - not box fetch : (
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
  // 12secs w present config to draw ~240k pixels (960k samples -> 80k samples / thread / sec (that's ok!))
  // 1024x -> about 12 seconds to generate - not awful! (768pix in 30ms)

  // move to cpp: about 120k pixels per second (480k samples)
  // much more reasonable - wtf was i doing in rust to make it run so shit?????
  //
  // at higher scales, more paths: about 70k pixels / second (sample time of ~ 10us)
  // - 280k samples / second - would take 4secs for a single thread to crunch a pixel
  benchStart("draw_time");
  draw.WriteImage(glm::dvec2(-1024, -1560), glm::ivec2(4096, 4096), 15.0, "courses.jpg");
  benchEnd("draw_time");
  // chunk draw
  // call sample - don't mind the cost
}
