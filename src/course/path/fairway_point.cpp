#include "course/path/fairway_point.hpp"

namespace course {
  namespace path {
    fairway_point* fairway_point_alloc(int size) {
      fairway_point* res = new fairway_point;
      res->next = fairway_point_alloc(size - 1);

      return res;
    }

    void fairway_point_free(fairway_point* point) {
      fairway_point_free(point->next);
      delete point;
    }
  }
}