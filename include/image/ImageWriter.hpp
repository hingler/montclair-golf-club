#ifndef WRITE_TO_IMAGE_H_
#define WRITE_TO_IMAGE_H_

#include <memory>

#include "image/rgba_color.hpp"
#include "image/ColorUtils.hpp"
#include "image/TerrainToColorMap.hpp"
#include "image/GenericImage.hpp"

#include "stb_image_write.h"

#include <cinttypes>
#include <string>
#include <type_traits>

/**
 * @brief Test implementation for writing terrain data to images.
 */

namespace image {
  namespace imagewriter {
    void WriteImageToFile(const GenericImage<RGBA<uint8_t>>& data, const std::string& filename);

    template <typename InputType>
    void WriteImageToFile(const GenericImage<RGBA<InputType>>& data, const std::string& filename) {
      auto dims = data.Dimensions();
      // convert to uint8 -- should already be ordered and arranged here
      auto result_image = GenericImage<RGBA<uint8_t>>(dims.width, dims.height);
      RGBA<uint8_t> temp;

      for (int y = 0; y < dims.height; y++) {
        for (int x = 0; x < dims.width; x++) {
          colorutil::GetUintRGBA(*data.Get(x, y), temp);
          result_image.Set(x, y, temp);
        }
      }

      stbi_write_jpg(filename.c_str(), dims.width, dims.height, 4, result_image.Data(), 95);
    }
  
  }


}

#endif