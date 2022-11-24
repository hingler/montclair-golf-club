#ifndef INTERPOLATABLE_H_
#define INTERPOLATABLE_H_

#include <type_traits>

namespace traits {
  struct interpolatable_impl {
        template <typename DataType,
        typename AddOp = decltype(std::declval<DataType&>() + std::declval<DataType&>()),
        typename ScaleOp = decltype(std::declval<DataType&>() * 5.0f),
        typename DefaultOp = std::is_default_constructible<DataType>>
        static std::true_type test(int);

        template <typename DataType, typename...>
        static std::false_type test(...);
      };

      template <typename T>
      struct interpolatable : decltype(interpolatable_impl::test<T>(0)) {};
}

#endif