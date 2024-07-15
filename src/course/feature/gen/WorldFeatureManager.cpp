#include "course/feature/gen/WorldFeatureManager.hpp"
#include "course/feature/SamplerBuilder.hpp"
#include "course/feature/WorldFeature.hpp"
#include "course/feature/WorldFeatureBuilder.hpp"
#include "course/feature/WorldFeatureInstancer.hpp"
#include "course/feature/WorldFeatureOverlapTester.hpp"
#include "course/feature/WorldFeaturePositioner.hpp"
#include "course/feature/gen/FeatureInstanceSpawner.hpp"
#include "course/feature/gen/FeatureSubSampler.hpp"
#include "course/feature/height/UnderlyingSamplerHeight.hpp"
#include <algorithm>

namespace mgc {
  namespace mgc_course {
    namespace mgc_gen {
      double WorldFeatureManager::Sample(double x, double y, double underlying) const {
        // add underlying, so that we handle smoothing correctly
        double acc = underlying;
        for (const auto& sampler : samplers) {
          acc += sampler.second->Sample(x, y, acc);
        }

        // subtract "underlying" at the end
        return acc - underlying;
      }

      glm::vec4 WorldFeatureManager::SampleSplat(double x, double y, size_t index) const {
        // how to normalize this?
        glm::vec4 acc(0.0f);
        for (const auto& sampler : samplers) {
          acc += sampler.second->SampleSplat(x, y, index);
        }

        return acc;
      }

      float WorldFeatureManager::SampleGrassFill(double x, double y) const {
        float acc = 0.0f;
        for (const auto& sampler : samplers) {
          acc += sampler.second->SampleGrassFill(x, y);
        }

        acc = glm::clamp(acc, 0.0f, 1.0f);
        return acc;
      }

      float WorldFeatureManager::SampleTreeFill(double x, double y) const {
        float acc = 0.0f;
        for (const auto& sampler : samplers) {
          acc += sampler.second->SampleTreeFill(x, y);
        }

        acc = glm::clamp(acc, 0.0f, 1.0f);
        return acc;
      }

      double WorldFeatureManager::SampleOcclusion(double x, double y) const {
        double acc = std::numeric_limits<double>::max();
        for (const auto& sampler : samplers) {
          acc = std::min(acc, sampler.second->SampleOcclusion(x, y));
        }

        return acc;
      }

      void WorldFeatureManager::AddFeature(
        const SamplerBuilder& builder,
        FeatureId id,
        size_t chunk_size
      ) {
        AddFeature(
          builder.GetBuilder(),
          builder.GetPositioner(),
          id,
          chunk_size
        );
      }

      // with this flow: instancers are kept separate from build/pos
      // idea:
      // - remove instancer - work with only builder + positioner
      // - associate "feature" with a flag demarcating it
      // - given an instancer and a flag (probably an enum!), return some sort of "populator" component
      // - populator contains positioner and instancer, and can be used to reify instances by position
      // alt:
      // - pass the instancer in down the line, associating it with the desired flag
      // - worldfeaturemanager will update it on our behalf
      // - (issue: requires positional data, which introduces a coupling problem we don't want)
      //
      // liking the OG idea
      // implementation:
      // - strip instancer out of this equation
      // - (use builder to demarcate instances?)
      // - (tough to rip out i think)
      //
      // - use a set of "known" flags to associate builders+positioners with a fetchable ID
      // - after the fact, call functionality on CG_M to generate a cmpt which, given a position, will realize instances
      // - pass instancer to this cmpt
      //
      // requires some coupling btwn builder and the game code...
      // ...but honestly i'm not distraught about that.
      //
      // tba:
      // - remove instancer from feature code
      // - move back to builder configuration
      // - use flags to supply instancer after the fact
      // - ATN will pass updates to instancer cmpt
      // - (probably: delegate a specific cmpt)
      // - (could write configuration st we can construct instances down the line)
      // - wc: initialize in our custom code, flags are a simple enum
      // - ergo: features are just builders and positioners, with a flag
      // - down the line, we fetch a feature (given an instancer) and use that to populate our models
      void WorldFeatureManager::AddFeature(
        const std::shared_ptr<WorldFeatureBuilder>& builder,
        const std::shared_ptr<WorldFeaturePositioner>& positioner,
        FeatureId id,
        size_t chunk_size
      ) {

        std::vector<std::shared_ptr<WorldFeatureSampler>> tester_samplers;
        for (const auto& pair : samplers) {
          tester_samplers.push_back(pair.second);
        }

        auto tester = std::make_shared<WorldFeatureOverlapTester>(
          tester_samplers
        );

        auto underlying_copy = std::make_shared<WorldFeatureManager>(*this);
        auto sampler_wrap = std::make_shared<UnderlyingSamplerHeight>(
          base_height,
          underlying_copy
        );

        // pass in shallow copy
        auto sampler = std::make_shared<WorldFeatureSampler>(
          positioner, builder, tester, sampler_wrap, chunk_size
        );

        samplers.insert(std::make_pair(id, sampler));
      }

      std::shared_ptr<FeatureInstanceSpawner> WorldFeatureManager::Spawn(
        const std::shared_ptr<WorldFeatureInstancer>& instancer,
        FeatureId id
      ) {
        auto itr = samplers.find(id);
        if (itr != samplers.end()) {
          auto positioner = itr->second->GetPositioner();
          return std::make_shared<FeatureInstanceSpawner>(
            positioner,
            instancer
          );
        }

        gog43::print("couldn't find sampler for instance: ", id);
        return nullptr;
      }

      std::shared_ptr<FeatureSubSampler> WorldFeatureManager::Prepare(
        const glm::dvec2& origin,
        const glm::dvec2& size
      ) const {
        WorldFeatureSampler::output_type output;
        for (const auto& sampler : samplers) {
          sampler.second->GetFeatures(origin, size, output);
        }

        // std::vector<std::shared_ptr<const WorldFeature>> features;
        std::vector<std::shared_ptr<const WorldFeature>> features(output.begin(), output.end());
        return std::make_shared<FeatureSubSampler>(features);
      }

      std::shared_ptr<FeatureSubSampler> WorldFeatureManager::Prepare(
        const chunker::ChunkIdentifier& id
      ) const {
        glm::dvec2 origin(id.x, id.y);
        glm::dvec2 size(glm::dvec2(id.sample_dims) * id.scale.AsDouble());

        return Prepare(origin, size);
      }
    }
  }
}
