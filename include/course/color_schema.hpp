#ifndef COLOR_SCHEMA_H_
#define COLOR_SCHEMA_H_

#include <cinttypes>

#include "course/terrain_type.hpp"
#include "image/rgba_color.hpp"

namespace course {
  /**
   * @brief Defines color of each terrain type for a given terrain cell (RGBA, 8bpc)
   * 
   */
  struct color_schema {
    int terrain_color[NUM_ELEMENTS];
  };
}

#endif