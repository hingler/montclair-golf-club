#ifndef GAUSSIAN_FAIRWAY_CONFIG_H_
#define GAUSSIAN_FAIRWAY_CONFIG_H_

#include <glm/glm.hpp>

namespace course {
  namespace sampler {
    namespace gaussian {
      struct GaussianTeeConfig {
        /* Tee Configuration */

        // probability that the tee is filled as a node
        double tee_fill_probability = 0.5;

        // probability that, if the tee is filled, it is joined to the rest of the course
        double tee_join_probability = 0.4;

        // size of the tee box itself, if not filled
        glm::vec2 tee_box_size = glm::vec2(10.0, 6.0);
      };

      struct GaussianNodeConfig {
        /* Node Fill */

        // sigma for nodes
        double node_sigma = 32.0;

        // intensity multiplier for nodes
        double intensity = 8.0;
      };

      struct GaussianFairwayConfig {
        /* Fairway fill */
        
        // mean sigma for path fill
        double mean_node_sigma = 32.0;

        // intensity multiplier for path fill
        double intensity = 0.3;

        // density of fill balls
        int density = 3;
      };

      struct GaussianFairwayScatterConfig {
        /* Controls scatter behavior along filled fairway path */

        // density of scatter balls
        int density = 3;

        // mean scatter distance from path center
        double mean_distance = 15.0;

        // mean intensity for scatter fill
        double intensity = 0.15;

        // sigma value for scatter fill
        double mean_scatter_sigma = 16.0;
      };

      struct GaussianSandConfig {
        // factor affecting number of bunkers to generate - larger = more sand
        double generation_factor = 6.0;

        // factor representing how tightly bunkers should generate around course nodes
        // 1.0 = bunkers generate tightly at course nodes, 0.0 = generate anywhere
        double tightness = 0.45;

        // number of initial samples to scatter
        int coarse_density = 4;

        // distance from init center to place coarse scatter cells
        double coarse_scatter_dist = 24.0;

        // number of samples to place in sand, about our initial samples
        int fine_density = 3;

        // lower = more accurate, higher = faster
        double step_factor = 2.0;

        // min sampler height at which we can place sand along fairway patches
        double gradient_threshold = 1.0;

        // min sampler height when placing sand patches near the green - should favor impeding fairway
        double green_gradient_threshold = 1.8;

        // sigma for divot fill
        double sigma = 12.0;

        // intensity of divot fill
        double divot_intensity = 0.67;

        // deviation of sand from center
        double fine_scatter_radius = 10.0;
      };

      // can work with this to start building test courses
      // what format should we be returning?
      // - heightmap
      // - surface feature
      // - should be "dynamic" st we can figure out what goes where later

      // how do we get surface features from hazard map?
      // - right now it's just sand - set a threshold and fill
      // - keep sand and water separate, set a threshold and fill in everything below that (ex. -0.5)
      // - we can "merge" - funnel sand shader into main for features curve, keep sand separate
      // - same for water :)
      //   - (will help with terrain gen too)

      // how do we generate terrain map?
      // - start with a base "safe map" wherever the course is - we'll know what needs to be smoothed by gradient magnitude
      // - safe map should introduce minor perturbations
      //   - introduce larger order terrain changes - step out influence as we move away from course
      //   - reit - nothing major for now, we can use gradient to power changes too
      // - we want to smooth out the green in particular, as much as is possible
      struct GaussianPathConfig {
        GaussianTeeConfig tee_config;
        GaussianNodeConfig node_config;
        GaussianFairwayConfig fairway_config;
        GaussianFairwayScatterConfig scatter_config;
        GaussianSandConfig sand_config;
      };
    }
  }
}

#endif