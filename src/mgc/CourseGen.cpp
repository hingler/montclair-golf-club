#include "mgc/CourseGen.hpp"

#include "course/sampler/gaussian/GaussianConfig.hpp"
#include "course/sampler/gaussian/GaussianFairwaySampler.hpp"
#include "course/sampler/gaussian/GaussianSandSampler.hpp"

#include "course/sampler/SimplexNoiseSampler.hpp"

#include "course/generator/BruteForceCourseGenerator.hpp"

#include "course/path/CoursePathToCurve.hpp"

using namespace course;
using namespace sampler;
using namespace gaussian;
using namespace generator;

namespace mgc {
  CourseGen::CourseGen(
    uint64_t seed
  ) {
    course_sampler = std::make_shared<GaussianMetaballSampler>();
    
    auto feature_sampler = std::make_shared<GaussianMetaballSampler>();

    GaussianPathConfig config;

    // throwaway rn
    std::shared_ptr<sampler::SimplexNoiseSampler> terrainsampler = std::make_shared<sampler::SimplexNoiseSampler>(glm::vec3(1.0), 4);

    auto gen = BruteForceCourseGenerator();
    gen.seed = seed;
    auto path = gen.GenerateCourse(terrainsampler, glm::vec2(1024, 1024));

    path.Recenter(glm::vec2(0, 0), glm::vec2(1024, 1024));

    auto curve = CoursePathToCurve(path, 0.5);
    auto fairway_sampler = std::make_shared<GaussianFairwaySampler>(
      path,
      curve
    );

    fairway_sampler->Generate(config);
    
    auto sand_sampler = std::make_shared<GaussianSandSampler>(
      fairway_sampler,
      path,
      curve
    );

    sand_sampler->Generate(config);

    course_sampler->Merge(*fairway_sampler, 1.0);
    course_sampler->Merge(*sand_sampler, -1.0);

    feature_sampler->Merge(*fairway_sampler, 1.0);
    feature_sampler->Merge(*sand_sampler, 1.0);

    this->fairway_sampler = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(0.4, 16.0, 0.1, course_sampler);
    this->green_sampler = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(15.9, 1000.0, 0.2, course_sampler);
    this->sand_sampler = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(0.2, 500.0, 0.2, std::dynamic_pointer_cast<GaussianMetaballSampler>(sand_sampler));
    this->rough_sampler = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(-1000.4, 0.4, 0.1, course_sampler);
    // decrease intensity to affect sampling - ensure hazards override it
    this->rough_sampler->intensity = 0.1;

    std::mt19937_64 engine;
    engine.seed(arc4random());
    std::uniform_real_distribution<double> test(-32768.0, 32768.0);

    auto generator = std::make_shared<terrain::HillGenerator>();
    generator->cell_size = 2048.0;
    generator->intensity_min = 203.5;
    generator->intensity_max = 183.8;
    generator->hill_sigma = 968.0;
    generator->scatter = 0.7;
    generator->fill_probability = 1.0;
    generator->offset = glm::dvec2(test(engine), test(engine));
    generator->scale = glm::vec2(1.0, 1.7);
    generator->displacement.intensity = glm::dvec2(8.0, 8.0);
    generator->displacement.noise_scale = glm::vec2(128.0, 128.0);
    generator->displacement.octaves = 2;
    base_terrain = std::make_shared<terrain::CourseSmoother<terrain::HillGenerator, terrain::HillGenerator>>(
      generator,
      generator,
      feature_sampler,
      curve
    );

    // this should be it! just need to define the helpers
  }

  CourseGen::CourseTerrainPtr CourseGen::GetFairwaySampler() { return fairway_sampler; }
  CourseGen::CourseTerrainPtr CourseGen::GetGreenSampler() { return green_sampler; }
  CourseGen::CourseTerrainPtr CourseGen::GetSandSampler() { return sand_sampler; }
  CourseGen::CourseTerrainPtr CourseGen::GetRoughSampler() { return rough_sampler; }
  std::shared_ptr<CourseGen::HeightMapType> CourseGen::GetHeightMap() { return base_terrain; }
}