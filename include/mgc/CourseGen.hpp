#ifndef COURSE_GEN_H_
#define COURSE_GEN_H_

#include <memory>
#include <random>

#include "course/sampler/GaussianMetaballSampler.hpp"
#include "course/sampler/gaussian/GaussianSandSampler.hpp"
#include "course/path/CoursePath.hpp"
#include "course/path/CompoundCurve.hpp"
#include "course/sampler/ThresholdSampler.hpp"
#include "course/sampler/ArithmeticSampler.hpp"

#include "terrain/HillGenerator.hpp"
#include "terrain/CourseSmoother.hpp"
#include "course/sampler/TreeFillSampler.hpp"

#include <glm/glm.hpp>



namespace mgc {
  /**
   * @brief Wraps course generation and returns splats, samplers, height maps, etc...
   *
   */
  class CourseGen {
   public:
    typedef course::sampler::ThresholdSampler<course::sampler::GaussianMetaballSampler> CourseTerrainSampler;
    typedef std::shared_ptr<CourseTerrainSampler> CourseTerrainPtr;
    typedef std::shared_ptr<course::sampler::ArithmeticSampler<CourseTerrainSampler, CourseTerrainSampler>> SandSampler;
    typedef gdterrain::CourseSmoother<gdterrain::HillGenerator, gdterrain::HillGenerator, gdterrain::HillGenerator> HeightMapType;
    typedef course::sampler::TreeFillSampler<course::sampler::GaussianMetaballSampler> FillSamplerType;

    /**
     * @brief Construct a new Course Gen object
     *
     * @param seed - seed used to power course generation.
     */
    CourseGen(
      uint64_t seed
    );

    // for now: course generation should be pretty standard
    // we have fairway/green/sand samplers which we create inline
    // we then create threshold samplers (thinking we do so on the fly) to wrap that underlying data
    // we also have our height map (cache base + smoothed)

    // tba:
    // - move to new splat approach?
    // - now: we can bind one "index sampler" to four "splat locations"
    // - ie: dropping the need for splat manager altogether

    // it's all one type!!!

    /**
     * @brief Returns sampler which represents fairway
     *
     * @return std::shared_ptr<CourseTerrainSampler> - fairway sampler
     */
    CourseTerrainPtr GetFairwaySampler();

    /**
     * @brief Returns sampler representing green
     *
     * @return std::shared_ptr<CourseTerrainSampler>
     */
    CourseTerrainPtr GetGreenSampler();

    /**
     * @brief Returns sampler representing sand
     *
     * @return std::shared_ptr<CourseTerrainSampler>
     */
   SandSampler GetSandSampler();

    /**
     * @brief Returns sampler representing rough
     *
     * @return std::shared_ptr<CourseTerrainSampler>
     */
    CourseTerrainPtr GetRoughSampler();

    /**
     * @brief Returns sampler representing tree fill
     *
     * @return TreeFillSampler
     */
     std::shared_ptr<CourseGen::FillSamplerType> GetTreeFillSampler();

    /**
     * @brief Returns course heightmap
     *
     * @return std::shared_ptr<HeightMapType>
     */
    std::shared_ptr<HeightMapType> GetHeightMap();

    course::path::CoursePath GetCoursePath();
    course::path::CompoundCurve GetCourseCurve();

    /**
     * @brief Returns estimate of bottom left corner of our course, as a yard-displacement from origin.
     *
     * @return glm::dvec2 - bottom left corner
     */
    glm::dvec2 GetCourseOrigin();

    /**
     * @brief Returns size of our course, in yards.
     *
     * @return glm::dvec2 - size, in yds
     */
    glm::dvec2 GetCourseSize();

    /**
     * @brief Returns an approximation of the course's center.
     *
     * @return glm::dvec3 - xyz coords for the course's center
     */
    glm::dvec3 GetCourseCenter();

    /**
     * @brief Get the seed for this course generator
     *
     * @return uint64_t - the seed in question:
     */
    uint64_t GetSeed();

   private:
    std::shared_ptr<course::sampler::GaussianMetaballSampler> course_sampler;
    course::path::CoursePath path;
    course::path::CompoundCurve curve;
    std::shared_ptr<gdterrain::CourseSmoother<gdterrain::HillGenerator, gdterrain::HillGenerator, gdterrain::HillGenerator>> base_terrain;
    // samplers
    CourseTerrainPtr fairway_sampler;
    CourseTerrainPtr green_sampler;
    CourseTerrainPtr rough_sampler;
    std::shared_ptr<FillSamplerType> tree_sampler;
    SandSampler sand_sampler;
    uint64_t seed_;
  };
}

#endif // COURSE_GEN_H_
