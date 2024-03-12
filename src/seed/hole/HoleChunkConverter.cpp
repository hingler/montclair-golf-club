#include "seed/hole/HoleChunkConverter.hpp"

#include <memory>

namespace mgc {
  HoleChunkConverter::HoleChunkConverter(
    const std::shared_ptr<HoleChunkManager>& manager,
    const ChunkConfig& config
  ) : manager(manager), config(config) {

  }
}
