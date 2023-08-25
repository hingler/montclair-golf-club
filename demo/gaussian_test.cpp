#include "course/sampler/GaussianMetaballSampler.hpp"
#include "image/TerrainImageConverter.hpp"

#include "image/ImageWriter.hpp"

#include "image/GenericImage.hpp"
#include "image/rgba_color.hpp"

#include "course/generator/BruteForceCourseGenerator.hpp"
#include "course/sampler/SimplexNoiseSampler.hpp"

#include "course/path/Rect.hpp"
#include "course/path/CoursePathToCurve.hpp"

#include "course/sampler/gaussian/GaussianFairwaySampler.hpp"
#include "course/sampler/gaussian/GaussianSandSampler.hpp"

#include "terrain/HillGenerator.hpp"
#include "terrain/CourseSmoother.hpp"

#include <algorithm>

using namespace course;
using namespace sampler;
using namespace gaussian;

using namespace path;

using namespace image;

int main(int argc, char** argv) {
  std::shared_ptr<sampler::SimplexNoiseSampler> terrainsampler = std::make_shared<sampler::SimplexNoiseSampler>(glm::vec3(1.0), 4);

  auto sampler = std::make_shared<GaussianMetaballSampler>();
  auto hazards = GaussianMetaballSampler();
  // todo: integrate with course builder and see if i can get anything interesting out of thiss

  auto dims = glm::ivec2(1024, 1024);

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

  GaussianFairwaySampler fairway_sampler(*sampler.get(), positions, curve);
  fairway_sampler.Generate(config);

  GaussianSandSampler sand_sampler(*sampler.get(), hazards, positions, curve);
  sand_sampler.Generate(config);

  sampler->Merge(hazards, -1.0);

  // thinking we split this into the "nodes" and "fill" agai

  auto generator = std::make_shared<terrain::HillGenerator>();

  // up close: this looks really really good!
  generator->cell_size = 1024.0;
  generator->intensity_min = 15.5;
  generator->intensity_max = 23.8;
  generator->hill_sigma = 484.0;
  generator->scatter = 0.7;
  generator->fill_probability = 1.0;
  generator->scale = glm::vec2(1.0, 1.7);
  generator->displacement.intensity = glm::dvec2(8.0, 8.0);
  generator->displacement.noise_scale = glm::vec2(128.0, 128.0);
  generator->displacement.octaves = 2;

  // want to build in golf course handling
  // - accept course as a feature map

  terrain::CourseSmoother smoother(generator, generator, sampler, curve);

  // can't get it right, but as far as noise goes it looks neat ish
  // really i want that "voronoi/gaussian" combination (nvm - i think this sort of like worley-deluxe approach is fun!)

  // oops! all i've done is re-invent worley noise :3

  GenericImage<RGBA<float>> output = converter::GaussianToRGBA(smoother, 0.5, 0.0, dims);
  image::imagewriter::WriteImageToFile(output, "gaussian.jpg");
}