#include "image/TerrainImageConverter.hpp"

namespace image {
  namespace converter {
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
