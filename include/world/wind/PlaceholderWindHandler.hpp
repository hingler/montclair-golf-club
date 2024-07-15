#ifndef PLACEHOLDER_WIND_HANDLER_H_
#define PLACEHOLDER_WIND_HANDLER_H_

#include "world/WindHandler.hpp"
namespace mgc {
  namespace world {
    class PlaceholderWindHandler : public WindHandler {
     public:
      PlaceholderWindHandler() : PlaceholderWindHandler(glm::dvec3(0.0)) {}

      PlaceholderWindHandler(const glm::dvec3& wind) : wind(wind) {}

      glm::dvec3 GetWind(const glm::dvec3& position) const override {
        return wind;
      }

     private:
      const glm::dvec3 wind;
    };
  }
}

#endif // PLACEHOLDER_WIND_HANDLER_H_
