#ifndef GROW_CONFIG_H_
#define GROW_CONFIG_H_

#include <random>

namespace mgc {
  struct GrowConfig {
    // 1.0: on branch, preserve direction. 
    // 0.0: no preservation
    double rigidity;

    // dist to step, per iteration
    double step_size;

    // approx branching dist
    double branch_probability;

    // approx angle to spawn branches at wrt main path
    double branch_angle_degs;

    // seed energy:
    // - specify a base "per branch" energy
    // - attempt to search out along branches

    // approx. base energy per init branch
    double base_energy;
  };
}

#endif // GROW_CONFIG_H_