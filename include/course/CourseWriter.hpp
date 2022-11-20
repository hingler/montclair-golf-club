#ifndef COURSE_WRITER_H_
#define COURSE_WRITER_H_

#include <glm/glm.hpp>

#include "course/sampler/ISampler.hpp"

#include "course/terrain_type.hpp"
#include "course/terrain_data.hpp"

#include "image/GenericImage.hpp"

#include <memory>
#include <unordered_map>

namespace course {
  namespace writer {
    /**
     * @brief Compiles a terrain image from a series of samplers.
     * 
     * @param samplers - maps terrain types to samplers.
     * @param image_dims - dimensions for resultant image.
     * @param terrain_dims - dimensions in course space to read.
     * @param terrain_center - center point for sampling.
     * @return image::GenericImage<terrain_data> - compiled image
     */
    image::GenericImage<terrain_data> GetCourseTerrainFromSamplers(
      const std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler<float>>>& samplers,
      const glm::ivec2& image_dims,
      const glm::vec2& terrain_dims,
      const glm::vec2& terrain_center
    );
  }
}

#endif // COURSE_WRITER_H_