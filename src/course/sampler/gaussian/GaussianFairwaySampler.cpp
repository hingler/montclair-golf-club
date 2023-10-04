#include "course/sampler/gaussian/GaussianFairwaySampler.hpp"

#include <glm/gtx/rotate_vector.hpp>

#include <cmath>
#include <random>

namespace course {
  using namespace path;
  namespace sampler {
    namespace gaussian {
      GaussianFairwaySampler::GaussianFairwaySampler(
        const CoursePath& course_path,
        const CompoundCurve& bezier_curve
        ) : path(course_path), curve(bezier_curve) 
      {
        engine.seed(arc4random());
      }

      void GaussianFairwaySampler::Generate(const GaussianPathConfig& config) {
        // prune paths based on config values
        Clear();

        if (path.course_path.size() <= 0 || curve.Length() < 0.1) {
          // generate nothing!
          return;
        }

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

        CreateGreenNode(
          path_local.course_path[path_local.course_path.size() - 1],
          config.green_config
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
          Add(
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

          Add(position.x, position.y, mean_sigma, ball_intensity * mean_sigma);

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
          Add(sample_point.x, sample_point.y, config.mean_scatter_sigma, ball_intensity * config.mean_scatter_sigma);
        }
      }

      void GaussianFairwaySampler::CreateGreenNode(
        const glm::vec2& green_point,
        const GaussianGreenConfig& config
      ) {
        std::normal_distribution<double> intensity_distribution(1.0, 0.1);
        std::uniform_real_distribution<double> theta_dist(0.0, 2.0 * M_PI);
        std::uniform_real_distribution<double> r_dist(0.0, 1.0);

        std::normal_distribution<double> scatter_dist(0.0, config.green_scatter_sigma);

        for (int i = 0; i < config.coarse_density; i++) {
          double theta = theta_dist(engine);
          double scatter = scatter_dist(engine);

          glm::vec2 center_point = green_point + glm::vec2(cos(theta) * scatter, sin(theta) * scatter);
          for (int j = 0; j < config.fine_density; j++) {
            double fine_theta = theta_dist(engine);
            double fine_scatter = scatter_dist(engine) * 0.6;
            glm::vec2 sample_point = center_point + glm::vec2(cos(theta) * fine_scatter, sin(theta) * fine_scatter);
            Add(sample_point.x, sample_point.y, config.green_sigma, intensity_distribution(engine) * config.green_intensity * config.green_sigma);
          }
        }
      }
    }
  }
}