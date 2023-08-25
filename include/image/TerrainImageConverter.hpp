#ifndef TERRAIN_IMAGE_CONVERTER_H_
#define TERRAIN_IMAGE_CONVERTER_H_

#include "image/GenericImage.hpp"
#include "course/terrain_data.hpp"
#include "rgba_color.hpp"
#include "TerrainToColorMap.hpp"

#include "type/sampler_type.hpp"

#include "course/sampler/GaussianMetaballSampler.hpp"

/**
 * @brief Uses a color mapping and a terrain image to convert terrain data to RGBA (float)
 */

namespace image {
  namespace converter {
    GenericImage<RGBA<float>> TerrainToRGBA(const GenericImage<course::terrain_data>& terrain_data, const TerrainToColorMap& color_map);

    template <typename SamplerType>
    GenericImage<RGBA<float>> GaussianToRGBA(SamplerType& sampler, double height_scale, double offset, const glm::ivec2& dims) {
      static_assert(type::sampler_type<SamplerType>::value);
      GenericImage<RGBA<float>> output(dims.x, dims.y);

      RGBA<float> temp;
      for (int y = 0; y < dims.y; y++) {
        for (int x = 0; x < dims.x; x++) {
          temp.r = sampler.Sample(x, y) * height_scale + offset;
          temp.g = temp.r;
          temp.b = temp.r;
          temp.a = 1.0;
          output.Set(x, y, temp);
        }
      }

      return output;
    }
  }
}

#endif // TERRAIN_IMAGE_CONVERTER_H_