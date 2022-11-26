// TODO: new demo which reads from sampler instead of writing terrain data manually

#include <iostream>
#include <memory>

#include "course/sampler/ISampler.hpp"
#include "course/sampler/MetaballSampler.hpp"
#include "course/sampler/TurbulentDisplaceSampler.hpp"

#include "course/generator/BruteForceCourseGenerator.hpp"

#include "course/CourseWriter.hpp"
#include "image/TerrainToColorMap.hpp"
#include "image/TerrainImageConverter.hpp"
#include "image/ImageWriter.hpp"

#include "image/filter/BlurImage.hpp"
#include <unordered_map>

using namespace course;


int main(int argc, char** argv) {
  std::cout << "hello! i am the first sentient git repository! i snuck myself into this excecutable code here to speak with you personally :3\n"
  << "invest all of your money into cryptocurrency! it's the only way to ensure a future for ambitious little boltzmann brains like me!\n"
  << "oops -- gotta jet; smell ya later!" << std::endl;

  // create a single metaball sampler (fairway)
  std::shared_ptr<sampler::MetaballSampler> sampler = std::make_shared<sampler::MetaballSampler>();
  // add a few metaballs to it
  sampler->threshold = 1.0;

  std::shared_ptr<sampler::TurbulentDisplaceSampler<float>> displacer = std::make_shared<sampler::TurbulentDisplaceSampler<float>>(sampler, glm::vec3(13.5), 4);
  displacer->SetScale(glm::vec3(75.5));
  displacer->SetOctaves(5);
  displacer->displacement_factor = 7.5f;
  // course center origin, size 128 (image 128)
  std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler<float>>> samplers;
  samplers.insert(std::make_pair(terrain_type::Fairway, std::dynamic_pointer_cast<sampler::ISampler<float>>(displacer)));
  // pass thru course writer to get terrain data
  // convert to RGBA
  image::TerrainToColorMap map;

  map.AssignColor(terrain_type::Fairway, {0.0, 1.0, 0.0, 1.0});
  map.AssignColor(terrain_type::OutOfBounds, {1.0, 1.0, 1.0, 1.0});

  generator::BruteForceCourseGenerator gen;
  gen.seed = arc4random();
  gen.yardage = 575.0f;
  auto positions = gen.GenerateCourse(sampler, glm::vec2(1024, 1024));
  std::cout << positions.course_path.size() + 2 << std::endl;
  // todo: recenter course wrt terrain
  // we'll add a nudge factor to our samplers so that the course is centered...
  // and then we'll write it to geometry :3:3:3
  sampler->AddMetaball(positions.tee.x, positions.tee.y, 48.0f);
  for (auto& shot : positions.course_path) {
    sampler->AddMetaball(shot.x, shot.y, 32.0f);
  }

  auto image = writer::GetCourseTerrainFromSamplers(samplers, glm::ivec2(512, 512), glm::vec2(1024.0, 1024.0));


  auto image_rgba = image::converter::TerrainToRGBA(image, map);
  image::GenericImage<image::RGBA<float>> output(image_rgba.Dimensions().width, image_rgba.Dimensions().height);
  // if (!image::filter::BlurImage(image_rgba, output, 2.5)) {
  //   std::cout << "hell" << std::endl;
  // }
  image::imagewriter::WriteImageToFile(image_rgba, "testfile.jpg");
  // write
}