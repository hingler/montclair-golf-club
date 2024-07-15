#include "seed/SeedPositioner.hpp"
#include "glm/fwd.hpp"
#include "seed/ChunkConfig.hpp"
#include <cmath>
#include <mutex>
#include <random>

namespace mgc {
  namespace seed {
    SeedPositioner::SeedPositioner(const ChunkConfig& config) : config(config) {}

    std::vector<glm::dvec2> SeedPositioner::GetSeeds(
      const glm::ivec2& chunk
    ) const {
      return GetSeeds(chunk, nullptr);
    }

    std::vector<glm::dvec2> SeedPositioner::GetSeeds(
      const glm::ivec2& chunk,
      std::vector<glm::ivec2>* index_output
    ) const {
      // how to do this??

      std::seed_seq s { chunk.x, chunk.y, 221144, 336699 };

      std::lock_guard lock(mutex);
      engine.seed(s);

      glm::dvec2 chunk_origin = static_cast<glm::dvec2>(chunk) * config.chunk_size;
      glm::dvec2 chunk_ceil = chunk_origin + glm::dvec2(config.chunk_size);

      glm::ivec2 seed_origin = glm::floor(chunk_origin / config.seed_dist);
      // prob shuffle around these positions a bit
      glm::ivec2 seed_ceil = glm::ceil(chunk_ceil / config.seed_dist);

      std::vector<glm::dvec2> res;


      for (int x = seed_origin.x; x <= seed_ceil.x; x++) {
        for (int y = seed_origin.y; y <= seed_ceil.y; y++) {
          glm::ivec2 seed_index(x, y);
          if (index_output != nullptr) {
            index_output->push_back(seed_index);
          }

          glm::dvec2 pos = GetSeedPosition(seed_index);

          if (ChunkContains(chunk, pos)) {
            res.push_back(pos);
          }
        }
      }

      return res;
    }

    // get seed indices and convert them?

    std::vector<glm::dvec2> SeedPositioner::GetSeeds(const glm::dvec2& origin, const glm::dvec2& size) const {
      return GetSeeds(origin, size, nullptr);
    }

    std::vector<glm::dvec2> SeedPositioner::GetSeeds(
      const glm::dvec2& origin,
      const glm::dvec2& size,
      std::vector<glm::ivec2>* index_output
    ) const {
      glm::ivec2 chunk_floor = glm::floor(origin / config.chunk_size);
      glm::ivec2 chunk_ceil = glm::ceil((origin + size) / config.chunk_size);

      std::vector<glm::dvec2> res;
      for (int x = chunk_floor.x; x <= chunk_ceil.x; x++) {
        for (int y = chunk_floor.y; y <= chunk_ceil.y; y++) {
          std::vector<glm::dvec2> chunk_seeds = GetSeeds(glm::ivec2(x, y), index_output);
          for (const auto& seed : chunk_seeds) {
            if (RegionContains(origin, size, seed)) {
              res.push_back(seed);
            }
          }
        }
      }

      return res;
    }

    glm::dvec2 SeedPositioner::GetSeedPosition(const glm::ivec2& seed_index) const {
      static std::uniform_real_distribution<double> theta(0.0, 2.0 * M_PI);
      static std::uniform_real_distribution<double> radius(0.0, 1.0);


      glm::dvec2 res(
        seed_index.x * config.seed_dist + (seed_index.x % 2) * (config.seed_dist / 2) + config.seed_origin.x,
        seed_index.y * config.seed_dist + config.seed_origin.y
      );

      double rad_sample = std::sqrt(radius(engine)) * 96.0;
      double theta_sample = theta(engine);

      res.x += rad_sample * std::cos(theta_sample);
      res.y += rad_sample * std::sin(theta_sample);

      return res;
    }

    bool SeedPositioner::ChunkContains(const glm::ivec2& chunk, const glm::dvec2& pos) const {
      return RegionContains(glm::dvec2(chunk) * config.chunk_size, glm::dvec2(config.chunk_size), pos);
    }

    bool SeedPositioner::RegionContains(const glm::dvec2& origin, const glm::dvec2& size, const glm::dvec2& pos) const {
      glm::dvec2 end = origin + size;
      return (
        pos.x > origin.x
        && pos.y > origin.y
        && pos.x <= end.x
        && pos.y <= end.y
      );
    }
  }
}
