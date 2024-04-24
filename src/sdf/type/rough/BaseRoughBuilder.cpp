#include "sdf/type/rough/BaseRoughBuilder.hpp"
#include "glm/fwd.hpp"
#include "sdf/CPPBundle.hpp"
#include "sdf/type/rough/BaseRoughSDF.hpp"
#include <memory>

namespace mgc {
  namespace rough {
    BaseRoughBuilder::BaseRoughBuilder() : sampler(2, 1.0, 512.0) {}

    std::shared_ptr<BaseRoughSDF> BaseRoughBuilder::Get(
      const CourseBundle& bundle,
      std::mt19937_64& engine
    ) const {
      // basically just fill everything in
      std::vector<std::shared_ptr<CPPBundle>> bundles;
      const auto& stops = bundle.path_stops;
      const auto& breaks = bundle.path_breaks;

      std::vector<glm::dvec2> points_working;
      std::vector<double> radii;
      std::shared_ptr<CPPBundle> sdf_bundle(nullptr);
      for (size_t i = 0; i < stops.size() - 1; i++) {
        // if (former) is not on break list: fill it in!
        if (std::find(breaks.begin(), breaks.end(), i) == breaks.end()) {
          if (sdf_bundle == nullptr) {
            sdf_bundle = std::make_shared<CPPBundle>();
          }

          size_t index_cursor = bundle.stop_indices[i];
          points_working.clear();
          radii.clear();

          while (index_cursor < bundle.stop_indices[i + 1]) {
            glm::dvec2 sample_point = bundle.course_path.at(index_cursor) + bundle.origin;
            points_working.push_back(bundle.course_path.at(index_cursor++));

            double radius_sample = sampler.Sample(sample_point.x, sample_point.y);
            // infeasibly large - should cover all cases (add to padding???)
            radii.push_back(radius_sample * 14.5 + 84.0);
          }

          sdf_bundle->AddCapsule(points_working, radii);
        } else if (sdf_bundle != nullptr) {
          // flush this bundle to array
          bundles.push_back(sdf_bundle);
          sdf_bundle = nullptr;
        }
      }

      if (sdf_bundle != nullptr) {
        // on loop complete: see if we can flush this bundle
        bundles.push_back(sdf_bundle);
      }

      // created capsules deposited in "bundles"
      // (use these bundles for smoothing hehe, as they should wrap)
      return std::make_shared<BaseRoughSDF>(
        bundles
      );
    }
  }
}
