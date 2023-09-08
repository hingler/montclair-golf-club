
#include "course/sampler/GaussianMetaballSampler.hpp"
#include "image/TerrainImageConverter.hpp"

#include "image/ImageWriter.hpp"

#include "image/GenericImage.hpp"
#include "image/rgba_color.hpp"

#include "course/generator/BruteForceCourseGenerator.hpp"
#include "course/sampler/SimplexNoiseSampler.hpp"
#include "course/sampler/ThresholdSampler.hpp"

#include "course/path/Rect.hpp"
#include "course/path/CoursePathToCurve.hpp"

#include "course/sampler/gaussian/GaussianFairwaySampler.hpp"
#include "course/sampler/gaussian/GaussianSandSampler.hpp"

#include <algorithm>
#include <memory>

// ambiguous namespace - clashing w terrain

using namespace course;
using namespace sampler;

using namespace path;

using namespace gaussian;
using namespace image;

int main(int argc, char** argv) {
  std::shared_ptr<SimplexNoiseSampler> terrainsampler = std::make_shared<SimplexNoiseSampler>(glm::vec3(1.0), 4);

  auto metaball_sampler = std::make_shared<GaussianMetaballSampler>();
  auto hazards = GaussianMetaballSampler();
  // todo: integrate with course builder and see if i can get anything interesting out of thiss

  auto dims = glm::ivec2(1024, 1024);

  // (tba: want to better delineate fairway sampler, etc because i don't like )

  generator::BruteForceCourseGenerator gen;
  gen.seed = arc4random();
  gen.yardage = 575.0f;

  const auto terrain_size = glm::vec2(1024, 1024);

  auto positions = gen.GenerateCourse(terrainsampler, terrain_size);
  Rect positions_bb;
  positions_bb.start = positions.course_path[0];
  positions_bb.end = positions.course_path[0];

  for (auto& position : positions.course_path) {
    positions_bb.start.x = std::min(position.x, positions_bb.start.x);
    positions_bb.start.y = std::min(position.y, positions_bb.start.y);
    positions_bb.end.x = std::max(position.x, positions_bb.end.x);
    positions_bb.end.y = std::max(position.y, positions_bb.end.y);
  }
  std::cout << positions.course_path.size() + 1 << std::endl;

  auto ideal_start = (terrain_size / 2.0f) - ((positions_bb.end - positions_bb.start) / 2.0f);
  for (auto& position : positions.course_path) {
    position += (ideal_start - positions_bb.start);
  }

  auto curve = CoursePathToCurve(positions, 0.5);

  GaussianPathConfig config;

  config.scatter_config.density = 6;
  // tba: can probably do this better, ie actually marching the gradient
  config.scatter_config.mean_distance = 56.0;
  config.scatter_config.intensity = 0.4;

  GaussianFairwaySampler fairway_sampler(positions, curve);
  fairway_sampler.Generate(config);

  GaussianSandSampler sand_sampler(fairway_sampler, positions, curve);
  sand_sampler.Generate(config);

  metaball_sampler->Merge(fairway_sampler, 1.0);
  metaball_sampler->Merge(sand_sampler, -1.0);

  // splat painting
  std::shared_ptr<ThresholdSampler<GaussianMetaballSampler>> threshold = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(0.4, 16.0, 0.1, metaball_sampler);

  GenericImage<RGBA<float>> output = converter::GaussianToRGBA(*threshold, 0.5, 0.0, dims);
  image::imagewriter::WriteImageToFile(output, "gaussian.jpg");
}