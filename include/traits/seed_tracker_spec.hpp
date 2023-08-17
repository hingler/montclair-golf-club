#ifndef SEED_TRACKER_H_
#define SEED_TRACKER_H_

#include <cinttypes>
#include <type_traits>

namespace traits {
  struct seed_tracker_spec_impl {
    template <typename SeedType,
      // PushSeed - accepts a bit depth (int) and a value to store (int) and records it
      typename PushSeed = std::is_same<void, decltype(std::declval<SeedType&>().PushSeed((uint32_t)0, (uint32_t)0))>,
      // PopSeed - accepts a bit depth (int) and returns the associated seed
      typename PopSeed = std::is_same<uint32_t, decltype(std::declval<SeedType&>().PopSeed((uint32_t)0))>,
      // CopySeed - copies data from another seed instance
      typename CopySeed = std::is_same<void, decltype(std::declval<SeedType&>().CopySeed(std::declval<const SeedType&>()))>>
    static std::true_type test(int);

    template <typename SeedType, typename...>
    static std::false_type test(...);
  };

  template <typename T>
  struct seed_tracker_spec : decltype(seed_tracker_spec_impl::test<T>(0)) {};
}

#endif // SEED_TRACKER_H_