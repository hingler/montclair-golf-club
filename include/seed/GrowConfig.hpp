#ifndef GROW_CONFIG_H_
#define GROW_CONFIG_H_

#include <random>

namespace mgc {
  struct GrowConfig {
    // 1.0: on branch, preserve direction.
    // 0.0: no preservation
    // per yard (ie rigidity 0.95 + 8 yard step -> (0.95 ^ 8))
    double rigidity = 0.99;

    // dist to step, per iteration
    double step_size = 2.0;

    // approx branching dist
    double branch_probability = -0.1;

    // approx angle to spawn branches at wrt main path
    double branch_angle_degs = 90.0;

    // seed energy:
    // - specify a base "per branch" energy
    // - attempt to search out along branches

    // approx. base energy per init branch
    double base_energy = 512.0;
  };
}

#endif // GROW_CONFIG_H_
