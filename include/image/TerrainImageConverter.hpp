#ifndef TERRAIN_IMAGE_CONVERTER_H_
#define TERRAIN_IMAGE_CONVERTER_H_

#include "image/GenericImage.hpp"
#include "course/terrain_data.hpp"
#include "rgba_color.hpp"
#include "TerrainToColorMap.hpp"

/**
 * @brief Uses a color mapping and a terrain image to convert terrain data to RGBA (float)
 */

namespace image {
  namespace converter {
    GenericImage<RGBA<float>> TerrainToRGBA(const GenericImage<course::terrain_data>& terrain_data, const TerrainToColorMap& color_map);
  }
}

#endif // TERRAIN_IMAGE_CONVERTER_H_