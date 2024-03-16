#ifndef TYPE_SAMPLER_H_
#define TYPE_SAMPLER_H_

/**
 * @brief Wrapper for sampler types
 *        Like heightmap, but floating point offsets.
 *        (ie what we're using in the course generator lole)
 */

#include <type_traits>

namespace mgc {
  namespace traits {
    namespace impl_ {
      struct sampler_type_impl {
        template <typename SamplerType,
        typename Sampler = std::is_same<double, decltype(std::declval<SamplerType&>().Sample(0.0, 0.0))>>
        static std::true_type test(int);

        template <typename SamplerType, typename...>
        static std::false_type test(...);
      };

      // tba: pretty sure this won't differentiate correctly but whatever
    }

    template <typename T>
    struct sampler_type : decltype(impl_::sampler_type_impl::test<T>(0)) {};
  }
}

#endif // TYPE_SAMPLER_H_
