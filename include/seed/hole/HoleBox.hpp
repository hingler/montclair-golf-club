#ifndef HOLE_BOX_H_
#define HOLE_BOX_H_

#include "corrugate/FeatureBox.hpp"

#include "util/LineUtil.hpp"

#include "seed/path/node/PathNode.hpp"

#include "gog43/Logger.hpp"

#include <algorithm>
#include <limits>
#include <vector>

namespace mgc {
  namespace _impl {
    struct HoleBox_Config {
      glm::dvec2 start;
      glm::dvec2 end;
      double padding;
    };
  }
  class HoleBox : public cg::FeatureBox {
   public:
    HoleBox(const PathNode* path, double padding, size_t seed) : HoleBox(HoleBox::GetPathPoints(path), padding, seed) {}
    HoleBox(const std::vector<glm::dvec2>& path, double padding, size_t seed) : HoleBox(HoleBox::GetConfig(path, padding), path, padding, seed) {}

    typedef std::vector<glm::dvec2>::const_iterator iterator;

    // true if within padding dist, false otherwise
    bool TestPadding(const glm::dvec2& point) const {
      glm::dvec2 point_local = point - GetOrigin();
      return TestPadding_local(point_local);
    }

    iterator begin() const {
      return path.cbegin();
    }

    iterator end() const {
      return path.cend();
    }

    const size_t seed;

   private:
    // swapped out falloff - doesn't matter here though
    const std::vector<glm::dvec2> path;
    const double padding;

    HoleBox(const _impl::HoleBox_Config& config, const std::vector<glm::dvec2>& path, double padding, size_t seed)
    : cg::FeatureBox(config.start, (config.end - config.start), 1.0f, 0.01f, 1.0f),
      path(HoleBox::MovePathToLocal(path, config.start)), padding(padding), seed(seed)
    {}

    static std::vector<glm::dvec2> GetPathPoints(const PathNode* input) {
      std::vector<glm::dvec2> output;
      output.reserve(input->size() + 1);
      for (size_t i = 0; i < input->size(); i++) {
        output.push_back(input->cat(i));
      }

      return output;
    }

    bool TestPadding_local(const glm::dvec2& point_local) const {
      double dist = std::numeric_limits<double>::max();
      for (size_t i = 1; i < path.size(); i++) {
        dist = std::min(dist, util::dist_to_line(point_local, path.at(i - 1), path.at(i)));
      }

      return (dist < padding);
    }

    // given a path in global coords and an origin, convert to local coordinates
    static std::vector<glm::dvec2> MovePathToLocal(const std::vector<glm::dvec2>& path, const glm::dvec2& origin) {
      std::vector<glm::dvec2> path_adjust;
      path_adjust.reserve(path.size() + 1);
      for (auto& point : path) {
        // move to box-local coords
        path_adjust.push_back(point - origin);
      }

      return path_adjust;
    }

    // given a path in global coords, convert path to box-local coords
    static _impl::HoleBox_Config GetConfig(const std::vector<glm::dvec2>& points, double padding) {
      _impl::HoleBox_Config res;
      res.start = glm::dvec2(std::numeric_limits<double>::max());
      res.end = glm::dvec2(std::numeric_limits<double>::lowest());

      for (auto& point : points) {
        res.start.x = glm::min(res.start.x, point.x);
        res.start.y = glm::min(res.start.y, point.y);
        res.end.x = glm::max(res.end.x, point.x);
        res.end.y = glm::max(res.end.y, point.y);
      }

      res.start -= glm::dvec2(padding);
      res.end += glm::dvec2(padding);

      return res;
    }
  };
}

#endif // HOLE_BOX_H_
