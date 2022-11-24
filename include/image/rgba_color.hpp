#ifndef RGBA_COLOR_H_
#define RGBA_COLOR_H_

#include <cinttypes>
#include <type_traits>

#include <cstring>

namespace image {

  template <typename ColorFormat>
  struct RGBA {
    static_assert(std::is_scalar_v<ColorFormat>);
    
    RGBA() {
      memset(this, 0, sizeof(RGBA<ColorFormat>));
    }

    RGBA(ColorFormat r_in, ColorFormat g_in, ColorFormat b_in, ColorFormat a_in) {
      r = r_in;
      g = g_in;
      b = b_in;
      a = a_in;
    }

    ColorFormat r;
    ColorFormat g;
    ColorFormat b;
    ColorFormat a;
  };

  template <typename ColorFormat>
  RGBA<ColorFormat> operator+(const RGBA<ColorFormat>& lhs, const RGBA<ColorFormat>& rhs) {
    RGBA<ColorFormat> res;
    res.r = lhs.r + rhs.r;
    res.g = lhs.g + rhs.g;
    res.b = lhs.b + rhs.b;
    res.a = lhs.a + rhs.a;

    return res;
  }

  template <typename ColorFormat>
  RGBA<ColorFormat> operator*(const RGBA<ColorFormat>& lhs, float rhs) {
    RGBA<ColorFormat> res;
    res.r = lhs.r * rhs;
    res.g = lhs.g * rhs;
    res.b = lhs.b * rhs;
    res.a = lhs.a * rhs;

    return res;
  }

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