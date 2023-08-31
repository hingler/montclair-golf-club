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

    struct int_sampler_type_impl {
      template <typename IntSamplerType,
      typename Sample = std::is_same<double, decltype(std::declval<IntSamplerType&>().Sample((int)0, (int)0))>>
      static std::true_type test(int);

      template <typename IntSamplerType, typename...>
      static std::false_type test(...);
    };
  }

  // wraps samplers w double precision
  template <typename SamplerType>
  struct sampler_type : decltype(_impl::sampler_type_impl::test<SamplerType>(0)) {};

  // wraps samplers w int precision
  template <typename IntSamplerType>
  struct int_sampler_type : decltype(_impl::int_sampler_type_impl::test<IntSsamplerType>(0)) {};
}

#endif // SAMPLER_TYPE_H_