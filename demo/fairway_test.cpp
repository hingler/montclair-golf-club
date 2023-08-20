
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

#include "course/sampler/grass/SimpleGrassSampler.hpp"
#include "course/sampler/grass/TeeSampler.hpp"

#include "course/path/Rect.hpp"

#include <memory>

using namespace course;
using namespace sampler;
using namespace path;
using namespace grass;

int main(int argc, char** argv) {
  image::TerrainToColorMap map;

  std::shared_ptr<sampler::SimplexNoiseSampler> sampler = std::make_shared<sampler::SimplexNoiseSampler>(glm::vec3(1.0), 4);

  map.AssignColor(terrain_type::Fairway, {0.0, 1.0, 0.0, 1.0});
  map.AssignColor(terrain_type::Rough, {0.0, 0.6, 0.6, 0.6});
  map.AssignColor(terrain_type::OutOfBounds, {1.0, 1.0, 1.0, 1.0});

  generator::BruteForceCourseGenerator gen;
  gen.seed = arc4random();
  gen.yardage = 575.0f;

  const auto terrain_size = glm::vec2(1024, 1024);

  auto positions = gen.GenerateCourse(sampler, terrain_size);

  Rect positions_bb;
  positions_bb.start = positions.course_path[0];
  positions_bb.end = positions.course_path[0];

  for (auto& position : positions.course_path) {
    positions_bb.start.x = std::min(position.x, positions_bb.start.x);
    positions_bb.start.y = std::min(position.y, positions_bb.start.y);
    positions_bb.end.x = std::max(position.x, positions_bb.end.x);
    positions_bb.end.y = std::max(position.y, positions_bb.end.y);
  }

  auto ideal_start = (terrain_size / 2.0f) - ((positions_bb.end - positions_bb.start) / 2.0f);
  for (auto& position : positions.course_path) {
    position += (ideal_start - positions_bb.start);
  }

  std::cout << positions.course_path.size() + 1 << std::endl;
  auto curve = path::CoursePathToCurve(positions, 0.5);

  std::shared_ptr<SimpleGrassSampler> fairway_sampler = std::make_shared<SimpleGrassSampler>(positions, curve, arc4random());
  // difficulty: rough simpler wrapping fairway metaballs doesn't work, as we miss parts of the path
  // doubly so for "heavy rough"
  // ++: can we cover an acceptable range w just metaballs?
  // prob want to combine approaches
  std::shared_ptr<SimpleGrassSampler> rough_sampler = std::make_shared<SimpleGrassSampler>(*fairway_sampler);
  
  rough_sampler->SetThresholdModifier(fairway_sampler->GetThresholdModifier() * 0.5);

  std::shared_ptr<MaxFloatSampler> join_sampler = std::make_shared<MaxFloatSampler>();

  std::shared_ptr<TeeSampler> tee_sampler = std::make_shared<TeeSampler>(positions);
  tee_sampler->length = 14.0f;
  tee_sampler->width = 24.0f;

  join_sampler->AddSampler(fairway_sampler);
  join_sampler->AddSampler(tee_sampler);
  
  std::unordered_map<terrain_type, std::shared_ptr<sampler::ISampler<float>>> samplers;
  samplers.insert(std::make_pair(terrain_type::Fairway, std::dynamic_pointer_cast<sampler::ISampler<float>>(join_sampler)));
  samplers.insert(std::make_pair(terrain_type::Rough, std::dynamic_pointer_cast<sampler::ISampler<float>>(rough_sampler)));

  auto image = writer::GetCourseTerrainFromSamplers(samplers, glm::ivec2(2048, 2048), glm::vec2(1024.0, 1024.0));
  auto image_rgba = image::converter::TerrainToRGBA(image, map);

  image::imagewriter::WriteImageToFile(image_rgba, "testfile.jpg");
}