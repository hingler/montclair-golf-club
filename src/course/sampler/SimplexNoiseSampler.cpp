#include "course/sampler/SimplexNoiseSampler.hpp"
#include "noise/simplex_noise.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace noise;

static glm::vec3 get_octave_offset(int octave_num) {
  auto dir = static_cast<glm::vec3>(calculate_grad(glm::ivec3(octave_num * 920711969)));
  dir = glm::normalize(dir);
  dir *= (glm::mod(static_cast<double>(int_hasher(octave_num * 694923787)), 1024.0) - 512.0);
  return dir;
}

static double get_octave_scale(int octave_num) {
  return pow(0.5, octave_num);
}

namespace course {
  namespace sampler {
    SimplexNoiseSampler::SimplexNoiseSampler(const glm::vec3& scale, int octaves)
    : scale_(scale), octaves_(octaves), time(0.0) { }

    float SimplexNoiseSampler::Sample(float x, float y) const {
      glm::dvec3 sample_point(static_cast<double>(x), static_cast<double>(y), time);
      sample_point *= scale_;
      
      float acc = 0.0;
      glm::dvec3 temp;

      for (int i = 0; i < octaves_; i++) {
        temp = sample_point;
        temp += get_octave_offset(i);
        temp *= get_octave_scale(i);
        // we need to generate a rotation matrix on the fly :(
        // lets just not do it >:)
        acc += simplex_3d(sample_point);
      }

      acc /= octaves_;
      return acc;
    }
  }
}