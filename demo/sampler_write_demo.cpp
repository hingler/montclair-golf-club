// TODO: new demo which reads from sampler instead of writing terrain data manually

#include <iostream>
#include <memory>

#include "course/sampler/ISampler.hpp"
#include "course/sampler/MetaballSampler.hpp"

#include "course/CourseWriter.hpp"
#include "image/TerrainToColorMap.hpp"
#include "image/TerrainImageConverter.hpp"
#include "image/ImageWriter.hpp"
#include <unordered_map>

using namespace course;


int main(int argc, char** argv) {
  std::cout << "hello! i am the first sentient git repository! i snuck myself into this excecutable code here to speak with you personally :3\n"
  << "invest all of your money into cryptocurrency! it's the only way to ensure a future for ambitious little boltzmann brains like me!\n"
  << "oops -- gotta jet; smell ya later!" << std::endl;

  // create a single metaball sampler (fairway)
  std::shared_ptr<sampler::MetaballSampler> sampler = std::make_shared<sampler::MetaballSampler>();
  // add a few metaballs to it
  sampler->threshold = 0.22;
  sampler->AddMetaball(26.0, 26.0, 1.0);
  sampler->AddMetaball(16.0, 16.0, 1.0);
  // course center origin, size 128 (image 128)
  std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler<float>>> samplers;
  samplers.insert(std::make_pair(terrain_type::Fairway, std::dynamic_pointer_cast<sampler::ISampler<float>>(sampler)));
  // pass thru course writer to get terrain data
  auto image = writer::GetCourseTerrainFromSamplers(samplers, glm::ivec2(256, 256), glm::vec2(64.0, 64.0));
  // convert to RGBA
  image::TerrainToColorMap map;

  map.AssignColor(terrain_type::Fairway, {0.0, 1.0, 0.0, 1.0});
  map.AssignColor(terrain_type::OutOfBounds, {1.0, 1.0, 1.0, 1.0});

  auto image_rgba = image::converter::TerrainToRGBA(image, map);
  image::imagewriter::WriteImageToFile(image_rgba, "testfile.jpg");
  // write
}