#include "sdf/type/fairway/BaseFairwayBuilder.hpp"

#include "gog43/Logger.hpp"
#include "sdf/Bundle.hpp"
#include "sdf/type/fairway/BaseFairwaySDF.hpp"
#include <algorithm>
#include <random>

namespace mgc {
  namespace fairway {
    BaseFairwayBuilder::BaseFairwayBuilder() : sampler(
      2,
      1.0,
      512.0
    ) {

    }
    std::shared_ptr<BaseFairwaySDF> BaseFairwayBuilder::Get(
      const CourseBundle& bundle,
      std::mt19937_64& engine
    ) const {
      // tee box (tba)
      const glm::dvec2& tee_center = bundle.course_path.at(0);
      const glm::dvec2& tee_direction = bundle.tee_direction;
      // 10x8 yards - may even be too big
      const glm::dvec2 tee_dimensions = glm::dvec2(8, 5);
      // first stop point circle

      // smooth w/in 16.0 yds i guess
      auto fairway_bundle = std::make_shared<CPPBundle>(25.0);

      // dist for path stop circle radius
      std::normal_distribution<double> circle_rad(17.0, 4.5);
      // wiggle for circles
      std::uniform_real_distribution<double> circle_wiggle(-24.0, 24.0);

      std::uniform_int_distribution<int> circle_count(3, 5);
      // draw capsules connecting points, with segments from bundle
      auto& stops = bundle.path_stops;
      for (size_t i = 1; i < stops.size(); i++) {
        // create some circles
        const glm::dvec2& stop = stops.at(i);

        int count = circle_count(engine);
        for (size_t r = 0; r < count; r++) {
          glm::dvec2 wiggle_pos = stop;
          wiggle_pos.x += circle_wiggle(engine);
          wiggle_pos.y += circle_wiggle(engine);
          double radius = std::max(circle_rad(engine), 11.0);
          fairway_bundle->AddCircle(wiggle_pos.x, wiggle_pos.y, radius);
        }
      }

      // start creating our course path
      // - want to pick up somewhere prior to our path
      CreateCapsules(
        bundle,
        engine,
        fairway_bundle
      );

      return std::make_shared<BaseFairwaySDF>(
        fairway_bundle,
        tee_center,
        tee_direction,
        tee_dimensions
      );
    }

    void BaseFairwayBuilder::CreateCapsules(
      const CourseBundle& bundle,
      std::mt19937_64& engine,
      const std::shared_ptr<CPPBundle>& output
    ) const {
      // what happens if ctrl point 16 is ahead of (16 behind stop 1)? undef'd b
      std::uniform_int_distribution<size_t> start_index(bundle.stop_indices.at(1) - 8, bundle.stop_indices.at(1));
      // work with an init index
      size_t index_cursor = std::max<size_t>(start_index(engine), 0);

      std::uniform_real_distribution<double> join_probability(0.0, 1.0);

      std::vector<glm::dvec2> points_working;
      std::vector<double> radii;

      for (size_t i = 1; i < bundle.stop_indices.size(); i++) {
        if (join_probability(engine) < 0.2) {
          // skip
          index_cursor = bundle.stop_indices.at(i);
          continue;
        }

        points_working.clear();
        radii.clear();

        while (index_cursor < bundle.stop_indices.at(i) && index_cursor < bundle.course_path.size()) {\

          glm::dvec2 sample_point = bundle.course_path.at(index_cursor) + bundle.origin;
          points_working.push_back(bundle.course_path.at(index_cursor));

          double sample = sampler.Sample(sample_point.x, sample_point.y);
          radii.push_back(sample * 11.5 + 41.0);

          index_cursor++;
        }

        output->AddCapsule(points_working, radii);
      }
    }
  }
}