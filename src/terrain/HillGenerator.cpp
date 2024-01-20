#define _USE_MATH_DEFINES

#include "terrain/HillGenerator.hpp"

#include "noise/simplex_noise.hpp"

#include "util/Gaussian.hpp"

#include <glm/gtx/hash.hpp>

#include <functional>
#include <cmath>

#define FALLOFF_BEGIN 1.5
#define FALLOFF_END 5.0



namespace gdterrain {
  using namespace _impl;

  HillInfo GenerateHillInfo(std::mt19937_64& engine, double scatter_radius, double intensity_min, double intensity_max, double fill_probability, double sigma_max) {
    // use the engine for this
    // issue: generation fails if we change params
    // - fix: use builder to prep vars on ctor
    // - not gonna deal w it now bc i dont need it to do anything weird yet

    // uh
    // need to speed this up a ton
    // - generate programmatically with a cheaper hashing function (ie not fetching random values from a big engine!!!)
    // - come up with a way to speed up fetches from map, or at least minimize concurrent accesses (probably the big cost)
    std::uniform_real_distribution<double> f_dist(0.0, 1.0);

    // random samples are costly!!!
    double theta     = f_dist(engine) * 2 * M_PI; 
    double intensity = f_dist(engine);
    double fill      = f_dist(engine);
    double half_scatter_radius = (scatter_radius / 2.0);
    HillInfo res;
    res.scatter_x = cos(theta) * half_scatter_radius;
    res.scatter_y = sin(theta) * half_scatter_radius;
    res.intensity = intensity * (intensity_max - intensity_min) + intensity_min;
    res.fill = (fill < fill_probability);
    res.local_sigma = (0.2 * f_dist(engine) + 0.8) * sigma_max;
    // does this work :3
    return res;
  }

  HillGenerator::HillGenerator() {}

  HillGenerator::HillGenerator(const HillGenerator& other) {
    intensity_min = other.intensity_min;
    intensity_max = other.intensity_max;

    // tba: changes to sampler don't propagate

    cell_size = other.cell_size;
    hill_sigma = other.hill_sigma;
    displacement = other.displacement;

    scatter = other.scatter;
    offset = other.offset;
    scale = other.scale;
    fill_probability = other.fill_probability;

    engine = other.engine;

    // cache only - should regen, no need to copy.
    hill_cache_concurrent = tbb::concurrent_hash_map<glm::ivec2, _impl::HillInfo>();
  }

  double HillGenerator::Sample(double x, double y) {
    // TODO: totally burned out on this. need to plug this into sampler output just to confirm that it looks ok :3
    // tba: is double compatible w our sampler?? i sure hope so :3
    double accumulator = 0.0;
    // calculate sample range - number of possible samples we have to compute
    int sample_range = static_cast<size_t>(ceil((hill_sigma * FALLOFF_END) / cell_size));
    // scale + offset
    glm::dvec2 sample_point = glm::dvec2((x + offset.x) / scale.x, (y + offset.y) / scale.y);
    // displace
    sample_point = displacement.Sample(sample_point);
    // calculate the cell we're in
    glm::ivec2 cell_offset = glm::ivec2(floor(sample_point.x / cell_size), floor(sample_point.y / cell_size));
    for (int x = -sample_range; x <= sample_range; x++) {
      for (int y = -sample_range; y <= sample_range; y++) {
        // calculate the cell we're sampling from
        glm::ivec2 local_offset = glm::ivec2(cell_offset.x + x, cell_offset.y + y);

        HillInfo info = LookupHill(local_offset);
        if (info.fill) {
          glm::dvec2 hill_center = glm::dvec2(
            local_offset.x * cell_size + info.scatter_x, local_offset.y * cell_size + info.scatter_y
          );

          glm::dvec2 d = (sample_point - hill_center);
          double dist_squared = (d.x * d.x + d.y * d.y);

          // scale up a bit more.
          double adjusted_intensity = info.intensity * info.local_sigma;
          // weird interference circles popping up on the fringes of shapes
          // (this is for sure the slowest element of generation - we can cut it a bit but it's always going to be slow)
          // (how can we optimize??)
          accumulator += util::GaussianFalloff(dist_squared, info.local_sigma, FALLOFF_BEGIN, FALLOFF_END) * adjusted_intensity;
        }
      }
    }

    // need to confirm that this works lole
    return accumulator;
  }

  // idea: do we want to make this thread safe?
  // (just need to synch hill generation and engine access)
  HillInfo HillGenerator::LookupHill(const glm::ivec2& offset) {
    // significant slow down - any way we can speed this up for threads???
    // tbb::concurrent_hash_map<glm::ivec2, HillInfo>::const_accessor hill;
    HillInfo res;
    auto itr = hill_cache.find(offset);
    // lookups are costly!!!
    // tbb: faster than before but not *fantastic*
    if (itr == hill_cache.end()) {
      auto seed = std::seed_seq(std::initializer_list<int>{ offset.x, offset.y });
      engine.seed(seed);

      double eff_scatter = scatter * cell_size;
      res = GenerateHillInfo(engine, eff_scatter, intensity_min, intensity_max, fill_probability, hill_sigma);
      
      {
        hill_cache.insert(std::make_pair(offset, res));
        // if (!hill_cache_concurrent.find(hill, offset)) {
        //   // confirm that someone else hasn't written this
        //   hill_cache_concurrent.insert(std::make_pair(offset, res));
        //   return res;
        // }
      }
    } else {
      res = itr->second;
    }

    // should be inserted if not already
    return res;
  }
}