#ifndef COLOR_UTILS_H_
#define COLOR_UTILS_H_

#include "image/rgba_color.hpp"

namespace image {
  // utilities for color formats
  // - scale and add
  // - convert between common formats
  // - etc

  template <typename InputType, typename IsFloating = std::enable_if<std::is_floating_point_v<InputType>>>
  void GetUintRGBA(const RGBA<InputType>& input, RGBA<uint8_t>& output) {
    output.r = static_cast<uint8_t>(input.r * 256.0);
    output.g = static_cast<uint8_t>(input.g * 256.0);
    output.b = static_cast<uint8_t>(input.b * 256.0);
    output.a = static_cast<uint8_t>(input.a * 256.0);
  }
  
  template <typename InputType, typename...>
  void GetUintRGBA(const RGBA<InputType>& input, RGBA<uint8_t>& output) {
    output.r = static_cast<uint8_t>(input.r);
    output.g = static_cast<uint8_t>(input.g);
    output.b = static_cast<uint8_t>(input.b);
    output.a = static_cast<uint8_t>(input.a);
  }
}

#endif // COLOR_UTILS_H_