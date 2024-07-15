#include "path/CourseBundle.hpp"

#include <glm/gtx/rotate_vector.hpp>

namespace mgc {

  glm::dvec2 CourseBundle::SampleAtIndex(double index) const {
    if (course_path.size() <= 0) {
      return glm::dvec2(0);
    }
    if (index <= 0) {
      return course_path.at(0);
    } else if (index >= course_path.size() - 1) {
      return course_path.at(course_path.size() - 1);
    } else {
      double t = glm::fract(index);
      size_t floor = static_cast<size_t>(index);
      size_t ceil = floor + 1;
      return course_path.at(floor) * (1.0 - t) + course_path.at(ceil) * t;
    }
  }

  double CourseBundle::IndexAtDist(double yards) const {
    size_t floor = 0;
    double yardage_acc = 0.0;
    double dist_last = 0.0;

    if (yards <= 0.0 && course_path.size() > 0) {
      return 0.0;
    }

    while (yardage_acc < yards && ++floor < course_path.size()) {
      dist_last = glm::length(course_path.at(floor - 1) - course_path.at(floor));
      yardage_acc += dist_last;
      // once floor = size, or yardage_acc > yards, we stop
    }

    if (yardage_acc < yards) {
      return static_cast<double>(course_path.size() - 1);
    }

    // overshot - floor will be equal to the end point in this case
    double t_sub = ((yardage_acc - yards) / dist_last);
    return floor - t_sub;
  }

  glm::dvec2 CourseBundle::SampleAtDist(double yards) const {
    double index = IndexAtDist(yards);
    return SampleAtIndex(index);
  }

  double CourseBundle::IndexToYardage(size_t index) const {
    double acc = 0.0;

    for (size_t i = 1; i <= index && i < course_path.size(); i++) {
      acc += glm::length(course_path.at(i) - course_path.at(i - 1));
    }

    return acc;
  }

  size_t CourseBundle::GetNearestIndex(const glm::dvec2& pos_local) const {
    double min_dist = std::numeric_limits<double>::max();
    size_t min_index = 0;
    for (size_t i = 0; i < course_path.size(); i++) {
      double cur_dist = glm::length(course_path.at(i) - pos_local);
      if (cur_dist < min_dist) {
        min_dist = cur_dist;
        min_index = i;
      }
    }

    return min_index;
  }

  glm::dvec2 CourseBundle::GetTangent(double index) const {
    if (index <= 0) {
      return glm::normalize(course_path.at(1) - course_path.at(0));
    } else if (index >= course_path.size() - 1) {
      return glm::normalize(course_path.at(course_path.size() - 1) - course_path.at(course_path.size() - 2));
    } else {
      size_t index_floor = static_cast<size_t>(index);
      size_t index_ceil = index_floor + 1;
      return glm::normalize(course_path.at(index_ceil) - course_path.at(index_floor));
    }
  }

  glm::dvec2 CourseBundle::GetNormal(double index) const {
    glm::dvec2 tan = GetTangent(index);
    return glm::rotate(tan, M_PI / 2);
  }

  glm::dvec2 CourseBundle::GetBoundingBoxStart() const {
    glm::dvec2 min_point = course_path.at(0);
    for (const auto& point : course_path) {
      min_point = glm::min(min_point, point);
    }

    return origin + min_point - glm::dvec2(padding);
  }

  glm::dvec2 CourseBundle::GetBoundingBoxEnd() const {
    glm::dvec2 max_point = course_path.at(0);
    for (const auto& point : course_path) {
      max_point = glm::max(max_point, point);
    }

    return origin + max_point + glm::dvec2(padding);
  }


}
