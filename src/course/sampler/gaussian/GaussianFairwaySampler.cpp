#include "course/sampler/gaussian/GaussianFairwaySampler.hpp"

#include <glm/gtx/rotate_vector.hpp>

#include <cmath>
#include <random>

namespace course {
  using namespace path;
  namespace sampler {
    namespace gaussian {
      GaussianFairwaySampler::GaussianFairwaySampler(
        GaussianMetaballSampler& underlying_sampler,
        const CoursePath& course_path,
        const CompoundCurve& bezier_curve
        ) : sampler(underlying_sampler), path(course_path), curve(bezier_curve) 
      {
        engine.seed(arc4random());
      }

      void GaussianFairwaySampler::Generate(const GaussianPathConfig& config) {
        // prune paths based on config values
        sampler.Clear();

        std::uniform_real_distribution<double> d_prob(0.0, 1.0);

        path::CoursePath path_local(path);
        path::CompoundCurve curve_local(curve);

        
        if (d_prob(engine) > config.tee_config.tee_fill_probability) {
          // do not fill tee
          path_local.course_path.erase(path_local.course_path.begin());
          curve_local.RemoveSegment(0);
        } else if (d_prob(engine) > config.tee_config.tee_join_probability) {
          // do not join tee to the rest of the course
          curve_local.RemoveSegment(0);
          // (better way to write this bool logic?)
        }

        CreateFairwayNodes(
          path_local, 
          curve_local,
          15,
          config.node_config.node_sigma,
          config.node_config.intensity
        );

        CreateFairwayPath(
          path_local,
          curve_local,
          config.fairway_config.density,
          config.fairway_config.mean_node_sigma,
          config.fairway_config.intensity
        );

        CreateFairwayScatter(
          curve_local,
          config.scatter_config
        );
      }

      void GaussianFairwaySampler::CreateFairwayNodes(
        const CoursePath& course_path,
        const CompoundCurve& bezier_curve,
        int density,
        double mean_sigma,
        double intensity
      ) {
        // focus on just the path
        auto& path = course_path.course_path;
        for (int i = 0; i < path.size(); i++) {
          auto& vertex = path[i];
          sampler.Add(
            static_cast<double>(vertex.x),
            static_cast<double>(vertex.y),
            mean_sigma,
            mean_sigma * intensity
          );
        }
      }

      void GaussianFairwaySampler::CreateFairwayPath(
        const CoursePath& course_path,
        const path::CompoundCurve& bezier_curve,
        int density,
        double mean_sigma,
        double intensity
      ) {
        std::normal_distribution<double> intensity_distribution(1.0, 0.15);
        double dist_covered = bezier_curve.Length();
        int metaball_count = static_cast<int>(density * dist_covered / 25.0);

        for (int i = 0; i < metaball_count; i++) {
          double sample_t = static_cast<double>(i) / metaball_count;
          glm::vec2 position = bezier_curve.Sample(sample_t);
          double ball_intensity = intensity_distribution(engine);

          sampler.Add(position.x, position.y, mean_sigma, ball_intensity * mean_sigma);

        }
      }

      void GaussianFairwaySampler::CreateFairwayScatter(
        const path::CompoundCurve& bezier_curve,
        const GaussianFairwayScatterConfig& config
      ) {
        std::normal_distribution<double> intensity_distribution(1.0, 0.15);
        std::uniform_real_distribution<double> t_distribution(0.0, 1.0);
        std::uniform_real_distribution<float> scatter_distribution(-config.mean_distance, config.mean_distance);


        double dist_covered = bezier_curve.Length();
        int metaball_count = static_cast<int>(config.density * dist_covered / 25.0);

        for (int i = 0; i < metaball_count; i++) {
          double sample_t = t_distribution(engine);
          glm::vec2 sample_point = bezier_curve.Sample(sample_t);
          glm::vec2 sample_tangent;
          if (sample_t > 0.999) {
            sample_tangent = sample_point + (sample_point - bezier_curve.Sample(sample_t - 0.001));
          } else {
            sample_tangent = bezier_curve.Sample(sample_t + 0.001);
          }

          glm::vec2 tangent = glm::normalize(sample_tangent - sample_point);
          glm::vec2 normal = glm::rotate(tangent, static_cast<float>(M_PI / 2));

          sample_point += normal * scatter_distribution(engine);

          double ball_intensity = intensity_distribution(engine) * config.intensity;
          sampler.Add(sample_point.x, sample_point.y, config.mean_scatter_sigma, ball_intensity * config.mean_scatter_sigma);
        }
      }
    }
  }
}