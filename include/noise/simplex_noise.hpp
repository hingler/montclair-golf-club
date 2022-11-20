// simplex noise is public baby lets go
// simple implementation for a quick benchmark

#ifndef SIMPLEX_NOISE_H_
#define SIMPLEX_NOISE_H_

#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <cinttypes>

namespace noise {
  static const float SIMPLEX_F = 0.33333333333;
  static const float SIMPLEX_G = 0.16666666667;

  static const glm::dvec3 SIMPLEX_F3 = glm::dvec3(SIMPLEX_F);
  static const glm::dvec3 SIMPLEX_G3 = glm::dvec3(SIMPLEX_G);

  static const glm::dvec3 SIMPLEX_ARRAY[3] = {
    glm::dvec3(1.0, 0.0, 0.0),
    glm::dvec3(0.0, 1.0, 0.0),
    glm::dvec3(0.0, 0.0, 1.0)
  };

  static const glm::ivec3 I_SIMPLEX_ARRAY[3] = {
    glm::ivec3(1, 0, 0),
    glm::ivec3(0, 1, 0),
    glm::ivec3(0, 0, 1)
  };

  // static function doesnt make sense here
  // we need a proper header probably
  static inline uint32_t int_hasher(const uint32_t& input) {
    uint32_t h = input;
    h *= 0xcc9e2d51;
    h = (h << 15) | (h >> 17);
    h *= 0x1b873593;
    return h;
  }

  static inline glm::dvec3 calculate_grad(const glm::ivec3& input) {
    glm::u32vec3 hash_vec(int_hasher(input.x), int_hasher(input.y), int_hasher(input.z));
    // need to mix up well :(
    glm::dvec3 res(static_cast<double>(hash_vec.x) / 1024.0, static_cast<double>(hash_vec.y) / 1024.0, static_cast<double>(hash_vec.z) / 1024.0);
    res = glm::fract(res) - glm::dvec3(0.5);
    return res;
  }

  // distribution used in shadertoy is [-0.5, 0.5]

  static double simplex_3d(glm::dvec3 input) {
    glm::dvec3 skew = input;
    double coordinate_sum = glm::dot(input, SIMPLEX_F3);
    skew.x += coordinate_sum;
    skew.y += coordinate_sum;
    skew.z += coordinate_sum;

    auto skew_floor = glm::floor(skew);
    const auto simplex_i0 = static_cast<glm::ivec3>(skew_floor);

    // calc coord from skew, subtract unskewed as subtracting (skew - skew . g3)
    auto x = input - skew_floor + glm::dot(skew_floor, SIMPLEX_G3);

    int max_index;
    if (x.x > x.y) {
      if (x.x > x.z) {
        max_index = 0;
      } else {
        max_index = 2;
      }
    } else {
      if (x.y > x.z) {
        max_index = 1;
      } else {
        max_index = 2;
      }
    }

    int second_max_index = (x[(max_index + 1) % 3] > x[(max_index + 2) % 3] ? (max_index + 1) % 3 : (max_index + 2) % 3);

    const glm::dvec3& simplex_d1 = SIMPLEX_ARRAY[max_index];
    const glm::dvec3 simplex_d2 = simplex_d1 + SIMPLEX_ARRAY[second_max_index];
    const glm::dvec3 simplex_d3 = glm::dvec3(1.0);

    const auto simplex_i1 = simplex_i0 + I_SIMPLEX_ARRAY[max_index];
    const auto simplex_i2 = simplex_i1 + I_SIMPLEX_ARRAY[second_max_index];
    const auto simplex_i3 = simplex_i0 + glm::ivec3(1);

    auto x1 = x - simplex_d1 + SIMPLEX_G3;
    auto x2 = x - simplex_d2 + 2.0 * SIMPLEX_G3;
    auto x3 = x - simplex_d3 + 3.0 * SIMPLEX_G3;

    // compute 
    glm::dvec4 res(glm::dot(x, x), glm::dot(x1, x1), glm::dot(x2, x2), glm::dot(x3, x3));
    res = glm::max(glm::dvec4(0.6) - res, glm::dvec4(0.0));

    glm::dvec4 grad;
    grad.x = glm::dot(x, calculate_grad(simplex_i0));
    grad.y = glm::dot(x1, calculate_grad(simplex_i1));
    grad.z = glm::dot(x2, calculate_grad(simplex_i2));
    grad.w = glm::dot(x3, calculate_grad(simplex_i3));

    // TODO: lots riffed from https://www.shadertoy.com/view/XsX3zB and https://en.wikipedia.org/wiki/Simplex_noise
    res *= res;
    res *= res;
    res *= grad;

    return glm::dot(res, glm::dvec4(52.0));
  }
} // namespace noise


#endif // SIMPLEX_NOISE_H_