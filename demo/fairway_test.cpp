
#include "course/sampler/ISampler.hpp"
#include "course/sampler/MetaballSampler.hpp"
#include "course/sampler/TurbulentDisplaceSampler.hpp"
#include "course/sampler/MaxFloatSampler.hpp"

#include "course/generator/BruteForceCourseGenerator.hpp"

#include "course/path/CoursePathToCurve.hpp"

#include "course/CourseWriter.hpp"
#include "image/TerrainToColorMap.hpp"
#include "image/TerrainImageConverter.hpp"
#include "image/ImageWriter.hpp"

#include "image/filter/BlurImage.hpp"

#include "course/sampler/fairway/SimpleFairwaySampler.hpp"
#include "course/sampler/fairway/TeeSampler.hpp"

#include <memory>

using namespace course;
using namespace sampler;
using namespace fairway;

int main(int argc, char** argv) {
  image::TerrainToColorMap map;

  std::shared_ptr<sampler::SimplexNoiseSampler> sampler = std::make_shared<sampler::SimplexNoiseSampler>(glm::vec3(1.0), 4);

  map.AssignColor(terrain_type::Fairway, {0.0, 1.0, 0.0, 1.0});
  map.AssignColor(terrain_type::OutOfBounds, {1.0, 1.0, 1.0, 1.0});

  generator::BruteForceCourseGenerator gen;
  gen.seed = arc4random();
  gen.yardage = 375.0f;
  
  auto positions = gen.GenerateCourse(sampler, glm::vec2(1024, 1024));
  std::cout << positions.course_path.size() + 1 << std::endl;
  auto curve = path::CoursePathToCurve(positions, 0.5);

  std::shared_ptr<SimpleFairwaySampler> fairway_sampler = std::make_shared<SimpleFairwaySampler>(positions, curve, arc4random());

  std::shared_ptr<MaxFloatSampler> join_sampler = std::make_shared<MaxFloatSampler>();

  std::shared_ptr<TeeSampler> tee_sampler = std::make_shared<TeeSampler>(positions);
  tee_sampler->length = 10.0f;
  tee_sampler->width = 24.0f;

  join_sampler->AddSampler(fairway_sampler);
  join_sampler->AddSampler(tee_sampler);
  
  std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler<float>>> samplers;
  samplers.insert(std::make_pair(terrain_type::Fairway, std::dynamic_pointer_cast<sampler::ISampler<float>>(join_sampler)));

  auto image = writer::GetCourseTerrainFromSamplers(samplers, glm::ivec2(1024, 1024), glm::vec2(1024.0, 1024.0));
  auto image_rgba = image::converter::TerrainToRGBA(image, map);

  image::imagewriter::WriteImageToFile(image_rgba, "testfile.jpg");
}