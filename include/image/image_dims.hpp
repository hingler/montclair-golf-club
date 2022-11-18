#ifndef IMAGE_DIMS_H_
#define IMAGE_DIMS_H_

#include <cinttypes>

namespace image {
  struct image_dims {
    uint32_t width;
    uint32_t height;

    image_dims(uint32_t width_, uint32_t height_) : width(width_), height(height_) {}
  };
}

#endif // IMAGE_DIMS_H_