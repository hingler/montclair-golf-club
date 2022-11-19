#ifndef TERRAIN_IMAGE_CONVERTER_H_
#define TERRAIN_IMAGE_CONVERTER_H_

#include "image/GenericImage.hpp"

#include "course/terrain_data.hpp"

/**
 * @brief Uses a color mapping and a terrain image to convert terrain data to RGBA (float)
 */

namespace image {
  namespace converter {
    GenericImage<RGBA<float>> TerrainToRGBA(const GenericImage<course::terrain_data>& terrain_data, const TerrainToColorMap& color_map);

    
    // IMPLEMENTATION

    GenericImage<RGBA<float>> TerrainToRGBA(const GenericImage<course::terrain_data>& terrain_data, const TerrainToColorMap& color_map) {
      auto dims = terrain_data.Dimensions();
      GenericImage<RGBA<float>> output(dims.width, dims.height);

      RGBA<float> temp;

      for (int y = 0; y < dims.height; y++) {
        for (int x = 0; x < dims.height; x++) {
          const auto* terrain = terrain_data.Get(x, y);
          color_map.CalculateColor(*terrain, temp);
          output.Set(x, y, temp);
        }
      }

      return output;
    }
  }
}

#endif // TERRAIN_IMAGE_CONVERTER_