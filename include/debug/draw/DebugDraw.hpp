#ifndef DEBUG_DRAW_H_
#define DEBUG_DRAW_H_

#include "debug/draw/impl/DrawCanvas.hpp"
#include "debug/draw/impl/DrawCanvasImpl.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "stb_image_write.h"

#include "gog43/Logger.hpp"

namespace mgc {
  // accept a bunch of RGBA samplers
  // write their output to an image
  class DebugDraw {
   public:
    template <typename SamplerType>
    void AddSampler(std::shared_ptr<SamplerType> sampler, const glm::dvec4& color) {
      canvas_list.push_back(std::make_shared<impl::DrawCanvasImpl<SamplerType>>(sampler, color));
    }

    // creating image:
    // - go in order, and compound
    // - 1 - alpha rule for compounding (dst * (1 - alpha) + src * alpha)
    // - init with black bg, and ignore alpha :3

    // tba:
    // - write a simple test sampler for seed paths
    //   - thinking: step point by point and look for closest line
    //     - will round our edges, which we want!
    //   - if dist to closest line is above some threshold then shade
    // - then: start working on the branching logic
    // (also: this approach is way way simpler than the old one lole)


    void WriteImage(const glm::dvec2& origin, const glm::ivec2& dims, double scale, const std::string& filename) {
      glm::dvec4* temp = new glm::dvec4[dims.x * dims.y];
      char* res = new char[dims.x * dims.y * 4];

      memset(temp, 0, dims.x * dims.y * 4 * sizeof(float));

      for (int y = 0; y < dims.y; y++) {
        gog43::print("writing y=", y);
        for (int x = 0; x < dims.x; x++) {
          glm::dvec2 coord(x * scale + origin.x, y * scale + origin.y);

          glm::dvec4 rolling_sample = glm::dvec4(0, 0, 0, 1);

          for (int sampler = 0; sampler < canvas_list.size(); sampler++) {
            glm::dvec4 col = canvas_list[sampler]->Sample(coord.x, coord.y);
            rolling_sample = rolling_sample * (1.0 - col.a) + glm::dvec4(col.r, col.g, col.b, 1.0) * col.a;
          }

          temp[y * dims.x + x] = rolling_sample;
        }
      }

      for (int y = 0; y < dims.y; y++) {
        for (int x = 0; x < dims.x; x++) {
          for (int c = 0; c < 4; c++) {
            res[(y * dims.x + x) * 4 + c] = static_cast<unsigned char>(glm::clamp(temp[y * dims.x + x][c], 0.0, 1.0) * 255.0);
          }
        }
      }

      // should be able to draw anything that we can sample now
      stbi_write_jpg(filename.c_str(), dims.x, dims.y, 4, res, 95);

      delete[] temp;
      delete[] res;
    }
   private:
    std::vector<std::shared_ptr<impl::DrawCanvas>> canvas_list;
  };
}
#endif // DEBUG_DRAW_H_