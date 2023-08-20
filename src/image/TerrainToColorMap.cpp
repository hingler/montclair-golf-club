#include "image/TerrainToColorMap.hpp"
#include "image/ColorUtils.hpp"

#include <cstring>

using namespace course;

namespace image {
  TerrainToColorMap::TerrainToColorMap() {
    memset(&colors, 0, sizeof(RGBA<float>) * terrain_type::NUM_ELEMENTS);
  }

  void TerrainToColorMap::AssignColor(terrain_type terrain, const RGBA<float>& color) {
    if (terrain >= 0 && terrain < terrain_type::NUM_ELEMENTS) {
      colors[terrain] = color;
    }
  }

  void TerrainToColorMap::CalculateColor(const course::terrain_data& terrain, RGBA<float>& output) const {
    output.r = 0.0f;
    output.g = 0.0f;
    output.b = 0.0f;
    output.a = 0.0f;

    float mag = 0.0;
    for (int i = 0; i < terrain_type::NUM_ELEMENTS; i++) {
      // current behavior: earlier terrain types take priority over later ones
      float intensity = (1.0 - mag) * terrain.terrain[i];
      colorutil::ScaleAndAdd(colors[i], intensity, output);
      mag += intensity;
    }
  }
}