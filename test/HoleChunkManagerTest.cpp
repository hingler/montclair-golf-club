#include <gtest/gtest.h>

#include "seed/hole/HoleChunkManager.hpp"
#include "seed/SeedGrower.hpp"
#include "seed/direct/DirectorBase.hpp"

#include <gog43/Logger.hpp>
#include <gog43/IOStreamLogger.hpp>
#include "seed/direct/CurlNoiseDirector.hpp"

using namespace gog43;

class HoleChunkManagerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    provide_logger(new IOStreamLogger());
  }
};

// "seems" to be working fine rn
// - need to write up an overlap tester ✅
// - need to confirm that it works as expected (test)
// - need to generate SDF holes (code in place)
// - need to generate samplers from those (easy enough - converter in place)
// once that's done: need to write up some orchestration logic, (tba)
// - mesh data + splat data currently run sample by sample
// - modify to utilize
// ...to bound it all up in something sampler-like

// current impl
// - hole chunk manager returns hole chunk boxes
// - overlap test receives neighbors, and sdf multisampler
// - generate holes, insert SDFs as we go
// - once SDFs are complete, then convert to samples and insert into final multisampler
// generation algo
// - optional function call (if supported) pre-caches chunk data for sampling
// - cache positions, then sample cache

using namespace mgc;

struct TestDirector : DirectorBase {
  TestDirector(double weight) : DirectorBase(weight) {}
 private:
  glm::dvec2 SampleDirection_impl(const DirectorInfo& context) const override {
    return glm::dvec2(0.707);
  }
};

TEST_F(HoleChunkManagerTest, SingleSeedTest) {
  auto grower = std::make_shared<SeedGrower>();
  grower->AddDirector<TestDirector>(1.0);

  ChunkConfig cc;
  cc.chunk_size = 512.0;
  cc.seed_dist = 2048.0;
  cc.seed_origin = glm::dvec2(256.0);

  GrowConfig gc;
  gc.base_energy = 256.0;
  gc.branch_probability = -0.1;
  gc.rigidity = 0.0;
  gc.step_size = 2.0;

  HoleChunkManager mgr(grower, cc, gc);

  // fetch chunk at 0, 0
  HoleChunkManager::output_type output;
  auto ptr = mgr.FetchNeighbors(glm::ivec2(0), output);

  glm::dvec2 chunk_start = ptr->GetOrigin();
  glm::dvec2 chunk_end   = ptr->GetEnd();

  // at least 1 hole
  ASSERT_GT(ptr->chunk_count(), 0);

  for (auto itr = ptr->begin(); itr != ptr->end(); itr++) {
    // ensure chunk box bounds all hole boxes tightly
    glm::dvec2 box_start = (*itr)->GetOrigin();
    glm::dvec2 box_end   = (*itr)->GetEnd();

    ASSERT_GE(box_start.x, chunk_start.x);
    ASSERT_GE(box_start.y, chunk_start.y);

    ASSERT_LE(box_end.x, chunk_end.x);
    ASSERT_LE(box_end.y, chunk_end.y);

    glm::dvec2 dist = *((*itr)->end() - 1) - *(*itr)->begin();
    dist = glm::normalize(dist);

    // should always move in the same direction, if rigidity is 0.
    ASSERT_NEAR(dist.x, 0.707, 0.001);
    ASSERT_NEAR(dist.y, 0.707, 0.001);

  }
  print("box count: ", ptr->chunk_count());
}

TEST_F(HoleChunkManagerTest, ScatterTest) {
  auto grower = std::make_shared<SeedGrower>();

  // curl noise takes almost all of the time
  grower->AddDirector<CurlNoiseDirector>(8, 2.0, 1024.0, 1.0);

  ChunkConfig cc;
  cc.chunk_size = 2048.0;
  cc.seed_dist = 513.0;
  // ensure all results remain bounded
  cc.seed_origin = glm::dvec2(256.0);

  GrowConfig gc;
  gc.base_energy = 1536.0;
  gc.branch_probability = 0.6;
  gc.rigidity = 0.98;
  gc.step_size = 4.0;

  HoleChunkManager mgr(grower, cc, gc);

  // fetch chunk at 0, 0
  HoleChunkManager::output_type output;
  // a little bit slow
  auto ptr = mgr.FetchNeighbors(glm::ivec2(0), output);

  glm::dvec2 chunk_start = ptr->GetOrigin();
  glm::dvec2 chunk_end   = ptr->GetEnd();

  // at least 1 hole
  ASSERT_GT(ptr->chunk_count(), 0);

  for (auto itr = ptr->begin(); itr != ptr->end(); itr++) {
    // ensure chunk box bounds all hole boxes tightly
    glm::dvec2 box_start = (*itr)->GetOrigin();
    glm::dvec2 box_end   = (*itr)->GetEnd();

    // impl assert_near
    ASSERT_GE(box_start.x, chunk_start.x - 0.00001);
    ASSERT_GE(box_start.y, chunk_start.y - 0.00001);

    ASSERT_LE(box_end.x, chunk_end.x + 0.00001);
    ASSERT_LE(box_end.y, chunk_end.y + 0.00001);
  }

  print("box count: ", ptr->chunk_count());
}
