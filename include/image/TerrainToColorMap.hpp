#ifndef TERRAIN_TO_COLOR_MAP_H_
#define TERRAIN_TO_COLOR_MAP_H_

#include "course/terrain_type.hpp"
#include "course/terrain_data.hpp"
#include "image/rgba_color.hpp"

namespace image {
  /**
   * @brief maps terrain types to colors, so they can be drawn onto a texture, image, etc.
   */
  class TerrainToColorMap {
    RGBA<float> colors[course::terrain_type::NUM_ELEMENTS];

  public:
    TerrainToColorMap();
    void AssignColor(course::terrain_type terrain, const RGBA<float>& color);
    void CalculateColor(const course::terrain_data& terrain, RGBA<float>& output) const;
  };
}

#endif