#include "image/ImageWriter.hpp"
#include "image/TerrainImageConverter.hpp"
#include "image/TerrainToColorMap.hpp"

#include "image/GenericImage.hpp"

#include "course/terrain_data.hpp"

using namespace course;
using namespace image;

int main(int argc, char** argv) {
  // need to generate a terrain image
  image::GenericImage<course::terrain_data> terrain(128, 128);
  course::terrain_data source;
  TerrainToColorMap map;

  map.AssignColor(terrain_type::Fairway, {0.0, 1.0, 0.0, 1.0});
  map.AssignColor(terrain_type::Green, {0.0, 0.0, 1.0, 1.0});
  map.AssignColor(terrain_type::Rough, {1.0, 0.0, 0.0, 1.0});
  map.AssignColor(terrain_type::Sand, {1.0, 1.0, 1.0, 1.0});
  // need to convert the terrain image to RGBA
  // need to write that image to a file

  for (int y = 0; y < 128; y++) {
    for (int x = 0; x < 128; x++) {
      memset(&source, 0, sizeof(terrain_data));
      source.terrain[(x >> 5) & 3] = 1.0;
      terrain.Set(x, y, source);
    }
  }

  auto image_data = converter::TerrainToRGBA(terrain, map);
  imagewriter::WriteImageToFile(image_data, "testfile.jpg");
}