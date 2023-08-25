#ifndef SAMPLER_TYPE_H_
#define SAMPLER_TYPE_H_

#include <type_traits>

// replacement for sampler interface

namespace type {
  namespace _impl {
    struct sampler_type_impl {
      template <typename SamplerType,
      typename Sample = std::is_same<double, decltype(std::declval<SamplerType&>().Sample((double)0.0, (double)0.0))>>
      static std::true_type test(int);

      template <typename MapType, typename...>
      static std::false_type test(...);
    };
  }

  template <typename SamplerType>
  struct sampler_type : decltype(_impl::sampler_type_impl::test<SamplerType>(0)) {};
}

#endif // SAMPLER_TYPE_H_