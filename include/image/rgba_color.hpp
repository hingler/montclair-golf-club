#ifndef RGBA_COLOR_H_
#define RGBA_COLOR_H_

#include <cinttypes>
#include <type_traits>

namespace image {

  template <typename ColorFormat>
  struct RGBA {
    static_assert(std::is_scalar_v<ColorFormat>);

    ColorFormat r;
    ColorFormat g;
    ColorFormat b;
    ColorFormat a;
  };

  struct rgba_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    static uint32_t rgba_to_color(const rgba_color& color);
    static void color_to_rgba(uint32_t input, rgba_color& output);
  };
}

#endif // RGBA_COLOR_H_