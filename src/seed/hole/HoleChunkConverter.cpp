#include "seed/hole/HoleChunkConverter.hpp"

#include <memory>

namespace mgc {
  HoleChunkConverter::HoleChunkConverter(
    const std::shared_ptr<HoleChunkManager>& manager,
    const ChunkConfig& config,
    const std::shared_ptr<mgc_course::mgc_gen::WorldFeatureManager>& pre_gen
  ) : manager(manager), config(config), pre_gen(pre_gen) {

  }
}
