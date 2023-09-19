#ifndef COURSE_GEN_H_
#define COURSE_GEN_H_

#include <memory>
#include <random>

#include "course/sampler/GaussianMetaballSampler.hpp"
#include "course/sampler/gaussian/GaussianSandSampler.hpp"
#include "course/path/CoursePath.hpp"
#include "course/path/CompoundCurve.hpp"
#include "course/sampler/ThresholdSampler.hpp"

#include "terrain/HillGenerator.hpp"
#include "terrain/CourseSmoother.hpp"

namespace mgc {
  /**
   * @brief Wraps course generation and returns splats, samplers, height maps, etc...
   * 
   */
  class CourseGen {
    typedef course::sampler::ThresholdSampler<course::sampler::GaussianMetaballSampler> CourseTerrainSampler;
    typedef std::shared_ptr<CourseTerrainSampler> CourseTerrainPtr;
    typedef terrain::CourseSmoother<terrain::HillGenerator, terrain::HillGenerator> HeightMapType;
   public:
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
   CourseTerrainPtr GetSandSampler();

    /**
     * @brief Returns sampler representing rough
     * 
     * @return std::shared_ptr<CourseTerrainSampler> 
     */
    CourseTerrainPtr GetRoughSampler();

    /**
     * @brief Returns course heightmap
     * 
     * @return std::shared_ptr<HeightMapType> 
     */
    std::shared_ptr<HeightMapType> GetHeightMap();

   private:
    std::shared_ptr<course::sampler::GaussianMetaballSampler> course_sampler;
    course::path::CoursePath path;
    course::path::CompoundCurve curve;
    std::shared_ptr<terrain::CourseSmoother<terrain::HillGenerator, terrain::HillGenerator>> base_terrain;

    // samplers
    CourseTerrainPtr fairway_sampler;
    CourseTerrainPtr green_sampler;
    CourseTerrainPtr rough_sampler;
    CourseTerrainPtr sand_sampler;
  };
}

#endif // COURSE_GEN_H_