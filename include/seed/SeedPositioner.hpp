#ifndef IG_SEED_POSITIONER_H_
#define IG_SEED_POSITIONER_H_

#include "seed/ChunkConfig.hpp"
#include <mutex>
#include <random>
#include <vector>
namespace mgc {
  namespace seed {
    // handles positioning individual seeds
    class SeedPositioner {
     public:
      SeedPositioner(const ChunkConfig& config);
      std::vector<glm::dvec2> GetSeeds(const glm::ivec2& chunk) const;
      std::vector<glm::dvec2> GetSeeds(const glm::ivec2& chunk, std::vector<glm::ivec2>* index_output) const;

      std::vector<glm::dvec2> GetSeeds(const glm::dvec2& origin, const glm::dvec2& size) const;
      std::vector<glm::dvec2> GetSeeds(const glm::dvec2& origin, const glm::dvec2& size, std::vector<glm::ivec2>* index_output) const;
     private:
      glm::dvec2 GetSeedPosition(const glm::ivec2& seed_index) const;
      bool ChunkContains(const glm::ivec2& chunk, const glm::dvec2& pos) const;
      bool RegionContains(const glm::dvec2& origin, const glm::dvec2& size, const glm::dvec2& pos) const;

      mutable std::mt19937_64 engine;
      ChunkConfig config;

      mutable std::mutex mutex;
    };

    // thinking: terrain will more frequently be generated within bounds
  }
}

#endif // IG_SEED_POSITIONER_H_
