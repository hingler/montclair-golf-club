#include "image/ImageWriter.hpp"


namespace image {
  namespace imagewriter {
    void WriteImageToFile(const GenericImage<RGBA<uint8_t>>& data, const std::string& filename) {
      auto dims = data.Dimensions();
      stbi_write_jpg(filename.c_str(), dims.width, dims.height, 4, data.Data(), 8);
    }
  }
}
