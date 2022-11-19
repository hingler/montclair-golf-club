#ifndef METABALL_H_
#define METABALL_H_

namespace course {
  namespace sampler {
    namespace impl {
      struct Metaball {
        // x position of this metaball
        float position_x;

        // y position of this metaball
        float position_y;

        // multiplied by distance to imitate radius
        float radius_factor;
      };
    }
  }
}

#endif // METABALL_H_