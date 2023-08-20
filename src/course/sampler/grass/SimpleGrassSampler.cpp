#include "course/sampler/grass/SimpleGrassSampler.hpp"

namespace course {
  
  using namespace path;

  namespace sampler {
    namespace grass {
      SimpleGrassSampler::SimpleGrassSampler(const CoursePath& course_path, const CompoundCurve& bezier_curve, uint64_t seed) {
        engine.seed(seed);

        underlying_sampler.threshold = 1.1f;

        CreatePatches_(course_path, bezier_curve, 30);
        FillPath_(course_path, bezier_curve);
      }

      void SimpleGrassSampler::CreatePatches_(const CoursePath& course_path, const CompoundCurve& bezier_curve, int density) {
        // visit points past 0
        // fill in a little bit of green around them
        // - establish a tangent line
        // - fill in a couple points around them (like 2)
        std::uniform_real_distribution<float> theta_dist(0.0f, 2 * M_PI);
        std::uniform_real_distribution<float> radius_dist(0.0f, 1.0f);
        glm::vec2 normal;
        glm::vec2 cross;
        auto& path = course_path.course_path;
        for (int i = 1; i < path.size(); i++) {
          normal = glm::normalize((i < path.size() - 1 ? path[i + 1] : path[i]) -  path[i - 1]);
          cross = glm::vec2(normal.y, -normal.x);

          for (int j = 0; j < density; j++) {
            float theta = theta_dist(engine);
            float r = 2.0f + 10.0f * sqrt(radius_dist(engine));
            glm::vec2 metaball_loc = path[i] + (cross * (cos(theta) * r) + normal * (sin(theta) * r * 2.1f));
            underlying_sampler.AddMetaball(metaball_loc.x, metaball_loc.y, 1.0f);
          }
        }
      }

      void SimpleGrassSampler::FillPath_(const CoursePath& course_path, const CompoundCurve& bezier_curve) {
        // create patches should ensure that our course is navigable
        // this method should fill in the bits in the middle
        std::uniform_real_distribution<double> f_dist(0.0, 1.0);
        double first_t = bezier_curve.GetTimeForEndOfSpecifiedSegment(0);
        double start_t = first_t * (0.5 + 0.35 * f_dist(engine));

        auto& path = course_path.course_path;
        FillRange_(bezier_curve, start_t, first_t, 4);
        for (int i = 2; i < path.size(); i++) {
          // possibly fill this in
          if (f_dist(engine) > 0.18) {
            double t_0 = bezier_curve.GetTimeForEndOfSpecifiedSegment(i - 2);
            double t_e = bezier_curve.GetTimeForEndOfSpecifiedSegment(i - 1);

            double t_range = t_e - t_0;
            t_0 += t_range * 0.18;
            t_e -= t_range * 0.18;
            FillRange_(bezier_curve, t_0, t_e, 3);
          }
        }
      }

      float SimpleGrassSampler::Sample(float x, float y) const {
        return underlying_sampler.Sample(x, y);
      }

      void SimpleGrassSampler::SetThresholdModifier(float threshold) {
        underlying_sampler.threshold = threshold;
      }

      float SimpleGrassSampler::GetThresholdModifier() {
        return underlying_sampler.threshold;
      }

      void SimpleGrassSampler::FillRange_(const path::CompoundCurve& bezier_curve, double min_t, double max_t, int density) {
        double dist_covered = bezier_curve.Length() * (max_t - min_t);
        // let's do density as "1.0radius metaballs per 25"
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        int metaball_count = static_cast<int>(ceil((density * dist_covered) / 25));

        glm::vec2 position, tangent, cross;
        for (int i = 0; i < metaball_count; i++) {
          double sample_t = min_t + dist(engine) * (max_t - min_t);

          // pick a position
          position = bezier_curve.Sample(sample_t);
          tangent = glm::normalize(bezier_curve.Sample(sample_t + 0.0001) - position);
          cross = glm::vec2(tangent.y, -tangent.x);

          // nudge along cross axis (we can add a width controller for this)
          float cross_dev = 16.0f * static_cast<float>(dist(engine)) - 8.0f;
          position += cross * cross_dev;

          // add metaball
          underlying_sampler.AddMetaball(position.x, position.y, 0.9f);
        }
      }
    }
  }
}