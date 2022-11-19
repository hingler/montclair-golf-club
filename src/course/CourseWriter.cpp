#include "course/CourseWriter.hpp"

namespace course {
  namespace writer {
    image::GenericImage<terrain_data> GetCourseTerrainFromSamplers(
      const std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler>>& samplers,
      const glm::ivec2& image_dims,
      const glm::vec2& terrain_dims,
      const glm::vec2& terrain_center
    ) {
      // priority is baked into enum order
      // from back to front:
      // - if sampler is present...
      // - clamp01 sample
      // - scale down terrain data by (1 - mag)
      // - add to appropriate terrain type

      image::GenericImage<terrain_data> output(image_dims.x, image_dims.y);
      float x_s = (image_dims.x > 1 ? terrain_dims.x / (image_dims.x - 1) : 0.0f);
      float x_i = terrain_center.x - (terrain_dims.x / 2) + x_s * 0.5;

      float y_s = (image_dims.y > 1 ? terrain_dims.y / (image_dims.y - 1) : 0.0f);
      float y_i = terrain_center.y - (terrain_dims.y / 2) + y_s * 0.5;

      glm::vec2 cur;
      terrain_data data;

      // initialize whole terrain as OOB so the math checks out
      // note: in that case we're skipping a whole sampler (will adjust later i guess to suit)
      data.terrain[terrain_type::OutOfBounds] = 1.0f;
      float temp;

      cur.y = y_i;
      for (int y = 0; y < image_dims.y; y++) {
        cur.x = x_i;
        for (int x = 0; x < image_dims.x; x++) {
          // a bit of a mess
          // luckily the number of samplers is small so we might get by
          for (int i = NUM_ELEMENTS - 2; i >= 0; i--) {
            auto itr = samplers.find(static_cast<terrain_type>(i));
            if (itr != samplers.end()) {
              // todo: there's a potential risk of cache clashes if we're constantly swapping btwn samplers
              // see if there's a quick way to populate a single sampler all at once
              // i think i rolled it back because we'd be fetching then writing cells over and over
              // and i liked the idea that we'd keep it local and write to the image just once
              // but might have to benchmark that :3:3:3 (writable interface?)
              auto ptr = itr->second;
              temp = glm::clamp(ptr->Sample(cur.x, cur.y), 0.0f, 1.0f);
              data.scale(1.0 - temp);
              data.terrain[i] = temp;
            }
          }

          cur.x += x_s;
        }

        cur.y += y_s;
      }

      return output;
    }
  }
}