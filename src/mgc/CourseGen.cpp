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

// need a lil bit of logic to establish bounds on the course itself (for course render)

namespace mgc {
  CourseGen::CourseGen(
    uint64_t seed
  ) {
    course_sampler = std::make_shared<GaussianMetaballSampler>();
    
    auto feature_sampler = std::make_shared<GaussianMetaballSampler>();

    GaussianPathConfig config;

    config.scatter_config.density = 6;
    // tba: can probably do this better, ie actually marching the gradient
    config.scatter_config.mean_distance = 56.0;
    config.scatter_config.intensity = 0.4;

    // throwaway rn
    std::shared_ptr<sampler::SimplexNoiseSampler> terrainsampler = std::make_shared<sampler::SimplexNoiseSampler>(glm::vec3(1.0), 4);

    BruteForceCourseGenerator gen;
    gen.seed = seed;
    gen.yardage = 525.0f;
    path = gen.GenerateCourse(terrainsampler, glm::vec2(1024, 1024));
    path.Recenter(glm::vec2(0, 0), glm::vec2(1024, 1024));

    // path.Recenter(glm::vec2(0, 0), glm::vec2(1024, 1024));

    curve = CoursePathToCurve(path, 0.5);
    auto fairway_sampler = std::make_shared<GaussianFairwaySampler>(
      path,
      curve
    );

    // - path is bvad
    // - sampling range is wrong
    // - 

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
    auto sand_threshold = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(0.2, 500.0, 0.2, std::dynamic_pointer_cast<GaussianMetaballSampler>(sand_sampler));
    auto course_threshold = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(-1000.0, -0.1, 0.05, course_sampler);
    this->sand_sampler = std::make_shared<ArithmeticSampler<ThresholdSampler<GaussianMetaballSampler>, ThresholdSampler<GaussianMetaballSampler>>>(sand_threshold, course_threshold);
    this->rough_sampler = std::make_shared<ThresholdSampler<GaussianMetaballSampler>>(-1000.4, 1000.0, 0.1, course_sampler);
    // decrease intensity to affect sampling - ensure hazards override it
    this->fairway_sampler->intensity = 1.0;
    this->rough_sampler->intensity = 0.2;

    std::mt19937_64 engine;
    engine.seed(arc4random());
    std::uniform_real_distribution<double> test(-32768.0, 32768.0);

    auto generator = std::make_shared<gdterrain::HillGenerator>();
    generator->cell_size = 256.0;
    generator->intensity_min = 65.5;
    generator->intensity_max = 255.8;
    generator->hill_sigma = 225.0;
    generator->scatter = 0.7;
    generator->fill_probability = 1.0;
    generator->offset = glm::dvec2(test(engine), test(engine));
    generator->scale = glm::vec2(1.0, 1.7);
    generator->displacement.intensity = glm::dvec2(24.0, 24.0);
    generator->displacement.noise_scale = glm::vec2(256.0, 256.0);
    generator->displacement.octaves = 2;

    auto generator_distant = std::make_shared<gdterrain::HillGenerator>();
    generator_distant->cell_size = 768.0;
    generator_distant->intensity_min = 752.5;
    generator_distant->intensity_max = 1225.6;
    generator_distant->hill_sigma = 945.2;
    generator_distant->scatter = 0.6;
    generator_distant->fill_probability = 0.84;
    generator_distant->displacement.intensity = glm::dvec2(48.0, 48.0);
    generator_distant->displacement.noise_scale = glm::vec2(512.0, 512.0);
    generator_distant->displacement.octaves = 2;

    base_terrain = std::make_shared<gdterrain::CourseSmoother<gdterrain::HillGenerator, gdterrain::HillGenerator, gdterrain::HillGenerator>>(
      generator,
      generator,
      generator_distant,
      feature_sampler,
      curve
    );

    // this should be it! just need to define the helpers
  }

  // when i wake up 

  CourseGen::CourseTerrainPtr CourseGen::GetFairwaySampler() { return fairway_sampler; }
  CourseGen::CourseTerrainPtr CourseGen::GetGreenSampler() { return green_sampler; }
  CourseGen::SandSampler      CourseGen::GetSandSampler() { return sand_sampler; }
  CourseGen::CourseTerrainPtr CourseGen::GetRoughSampler() { return rough_sampler; }
  std::shared_ptr<CourseGen::HeightMapType> CourseGen::GetHeightMap() { return base_terrain; }

  glm::dvec2 CourseGen::GetCourseOrigin() {
    return base_terrain->GetCourseOrigin();
  }

  glm::dvec2 CourseGen::GetCourseSize() {
    // tba: need some tests to confirm that these numbers are roughly correct
    return base_terrain->GetCourseSize();
  }

  course::path::CoursePath CourseGen::GetCoursePath() { return path; }
  course::path::CompoundCurve CourseGen::GetCourseCurve() { return curve; }
}