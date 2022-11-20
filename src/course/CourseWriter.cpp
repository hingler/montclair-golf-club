#include "course/CourseWriter.hpp"

namespace course {
  namespace writer {
    image::GenericImage<terrain_data> GetCourseTerrainFromSamplers(
      const std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler<float>>>& samplers,
      const glm::ivec2& image_dims,
      const glm::vec2& terrain_dims,
      const glm::vec2& terrain_center
    ) {
      image::GenericImage<terrain_data> output(image_dims.x, image_dims.y);
      float x_s = (image_dims.x > 1 ? terrain_dims.x / (image_dims.x - 1) : 0.0f);
      float x_i = terrain_center.x - (terrain_dims.x / 2) + x_s * 0.5;

      float y_s = (image_dims.y > 1 ? terrain_dims.y / (image_dims.y - 1) : 0.0f);
      float y_i = terrain_center.y - (terrain_dims.y / 2) + y_s * 0.5;

      glm::vec2 cur;
      terrain_data data;

      // initialize whole terrain as OOB so the math checks out
      // note: in that case we're skipping a whole sampler (will adjust later i guess to suit)
      
      float temp;

      cur.y = y_i;
      for (int y = 0; y < image_dims.y; y++) {
        cur.x = x_i;
        for (int x = 0; x < image_dims.x; x++) {
          // a bit of a mess
          // luckily the number of samplers is small so we might get by
          memset(&data, 0, sizeof(terrain_data));
          data.terrain[terrain_type::OutOfBounds] = 1.0f;
          for (int i = NUM_ELEMENTS - 2; i >= 0; i--) {
            auto itr = samplers.find(static_cast<terrain_type>(i));
            if (itr != samplers.end()) {
              auto ptr = itr->second;
              temp = glm::clamp(ptr->Sample(cur.x, cur.y), 0.0f, 1.0f);
              data.scale(1.0 - temp);
              data.terrain[i] = temp;
            }
          }

          output.Set(x, y, data);

          cur.x += x_s;
        }

        cur.y += y_s;
      }

      return output;
    }
  }
}