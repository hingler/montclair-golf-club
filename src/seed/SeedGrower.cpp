#include "seed/SeedGrower.hpp"

#include <algorithm>

#include <cmath>
#include <glm/gtx/rotate_vector.hpp>
#include <random>

#include <gog43/Logger.hpp>

using namespace gog43;

namespace mgc {
  SeedGrower::SeedGrower() {}

  std::vector<SeedGrower::path_ptr> SeedGrower::GeneratePaths(
    const glm::dvec2& init_point,
    const GrowConfig& config,
    std::seed_seq& sseq
  ) const {
    std::vector<std::shared_ptr<SeedPath>> path;

    // not sure how much of a chore it is to ctor this inline, but it saves us from dealing w multithread
    std::mt19937_64 engine;
    engine.seed(sseq);

    DirectorInfo info;
    info.point = init_point;
    info.root = init_point;

    double placeholder_theta = std::uniform_real_distribution<double>(0.0, 6.2829)(engine);
    info.direction = glm::dvec2(cos(placeholder_theta), sin(placeholder_theta));

    glm::dvec2 gradient = glm::normalize(SampleDirection(info));

    SeedPathBuilder builder(init_point);
    SeedPathBuilder reverse_builder(init_point);

    // need to test gradient
    // also: start writing directors :)
    GeneratePaths_recurse(builder, init_point, gradient, config.base_energy, config, engine);
    GeneratePaths_recurse(reverse_builder, init_point, -gradient, config.base_energy, config, engine);

    path.push_back(builder.Build());
    path.push_back(reverse_builder.Build());

    return path;
  }

  glm::dvec2 SeedGrower::SampleDirection(const DirectorInfo& info) const {
    glm::dvec2 res(0.0);
    for (auto& pt : directors) {
      res += pt->SampleDirection(info);
    }

    double len_squared = res.x * res.x + res.y * res.y;
    if (len_squared < (directors.size() * directors.size())) {
      res = glm::normalize(res);
    }

    return res;
  }

  glm::dvec2 SeedGrower::SampleDirection_WithRigidity(const DirectorInfo& info, const GrowConfig& config) const {
    glm::dvec2 director_sum = SampleDirection(info);
    double eff_rigidity = std::pow(config.rigidity, config.step_size);
    return glm::normalize(info.direction * eff_rigidity + director_sum * (1.0 - eff_rigidity));
  }

  #define MIN_ENERGY 180.0

  // testing this
  // - make sure paths are consistent with generation
  //   - create a director function which points in a known direction - ensure the path generally moves in that dir
  // - verify that rigidity constrains generation (ie: with rigidity at 1.0, path remains straight)

  void SeedGrower::GeneratePaths_recurse(
    SeedPathBuilder& builder,
    const glm::dvec2& init_point,
    const glm::dvec2& init_direction,
    double init_energy,
    const GrowConfig& global_config,
    std::mt19937_64& local_engine
  ) const {
    if (init_energy < 0.1) {
      // too little energy to continue - return
      // (if we're below min energy, then keep marching anyway)
      return;
    }

    DirectorInfo info;

    // tba: replace down the line
    std::uniform_real_distribution yardage_dist(MIN_ENERGY, 675.0);

    double travel_dist = yardage_dist(local_engine);
    travel_dist = std::min(travel_dist, init_energy);

    double dist_remaining = travel_dist;

    info.point = init_point;
    info.root = init_point;
    info.direction = init_direction;

    glm::dvec2 last_point = info.point - info.direction;

    while (dist_remaining > 0) {
      // normalize, with rigidity
      glm::dvec2 grad = SampleDirection_WithRigidity(info, global_config);

      last_point = info.point;
      info.point += grad * global_config.step_size;
      info.direction = glm::normalize(info.point - last_point);


      // march towards gradient
      builder.AddPoint(info.point);
      dist_remaining -= global_config.step_size;
    }

    double remaining_energy = init_energy - travel_dist;
    std::uniform_real_distribution branch_dist(0.0, 1.0);

    if (branch_dist(local_engine) < global_config.branch_probability) {

      // branch based on direction last traveled
      SeedPathBuilder branch_builder = builder.AddBranch();
      // pick some arb direction to go
      double branch_direction = (branch_dist(local_engine) > 0.5 ? 1.0 : -1.0);

      double branch_mid_rads = global_config.branch_angle_degs * M_PI / 180.0;

      // 0.8 -> 1.2x branch mid (that's ok for this?)
      double rotation_rads = (0.8 + 0.4 * std::pow(branch_dist(local_engine), 0.45)) * branch_direction * branch_mid_rads;
      glm::dvec2 new_direction = glm::rotate(info.direction, rotation_rads);

      // generate branch path
      GeneratePaths_recurse(
        branch_builder,
        info.point,
        new_direction,
        remaining_energy,
        global_config,
        local_engine
      );
    }

    // continue generating main path
    GeneratePaths_recurse(
      builder,
      info.point,
      info.direction,
      remaining_energy,
      global_config,
      local_engine
    );
  }


}
