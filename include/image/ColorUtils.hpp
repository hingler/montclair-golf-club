#ifndef COLOR_UTILS_H_
#define COLOR_UTILS_H_

#include "image/rgba_color.hpp"

namespace image {
  // utilities for color formats
  // - scale and add
  // - convert between common formats
  // - etc

  namespace colorutil {
    template <typename InputType, typename IsFloating = std::enable_if<std::is_scalar_v<InputType>>>
    void GetUintRGBA(const RGBA<InputType>& input, RGBA<uint8_t>& output) {
      if (std::is_floating_point_v<InputType>) {
        output.r = static_cast<uint8_t>(input.r * 256.0);
        output.g = static_cast<uint8_t>(input.g * 256.0);
        output.b = static_cast<uint8_t>(input.b * 256.0);
        output.a = static_cast<uint8_t>(input.a * 256.0);
      } else {
        output.r = static_cast<uint8_t>(input.r);
        output.g = static_cast<uint8_t>(input.g);
        output.b = static_cast<uint8_t>(input.b);
        output.a = static_cast<uint8_t>(input.a);
      }
    }

    template <typename InputType>
    void ScaleAndAdd(const RGBA<InputType>& input, float scale, RGBA<InputType>& output) {
      output.r += input.r * scale;
      output.g += input.g * scale;
      output.b += input.b * scale;
      output.a += input.a * scale;
    }
  }
}

#endif // COLOR_UTILS_H_