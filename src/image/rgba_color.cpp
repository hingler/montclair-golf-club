#include "image/rgba_color.hpp"

namespace image {  
  uint32_t rgba_color::rgba_to_color(const image::rgba_color& color) {
    return (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
  }

  void rgba_color::color_to_rgba(uint32_t input, image::rgba_color& output) {
    output.r = (input & 0xFF000000) >> 24;
    output.g = (input & 0x00FF0000) >> 16;
    output.b = (input & 0x0000FF00) >> 8;
    output.a = (input & 0x000000FF);
  }
}