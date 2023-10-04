#ifndef ARITHMETIC_SAMPLER_H_
#define ARITHMETIC_SAMPLER_H_

#include <memory>

#include "type/sampler_type.hpp"

namespace course {
  namespace sampler {
    enum ArithmeticMode {
      Multiply
    };
    template <typename sA, typename sB>
    class ArithmeticSampler {
      static_assert(type::sampler_type<sA>::value);
      static_assert(type::sampler_type<sB>::value);
     public:
      ArithmeticSampler(std::shared_ptr<sA> samplerA, std::shared_ptr<sB> samplerB) : sampler_a(samplerA), sampler_b(samplerB) {}
      double Sample(double x, double y) {
        switch (mode) {
          case Multiply:
            return sampler_a->Sample(x, y) * sampler_b->Sample(x, y);
        }
      }

      ArithmeticMode mode;
     private:
      std::shared_ptr<sA> sampler_a;
      std::shared_ptr<sB> sampler_b;
    };
  }
}

#endif // ARITHMETIC_SAMPLER_H_