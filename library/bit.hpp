#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BIT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BIT_HPP_

#include <macros.h>
#include <cassert>
#include <limits>
#include <type_traits>

template<typename int_t> inline int_t lowbit(int_t x) {
  return x & -x;
}

template<typename T> int countl_zero(T n) {
  static_assert(std::is_unsigned_v<T>,
                "countl_zero requires an unsigned integer type");
  if (n == 0)
    return std::numeric_limits<T>::digits;
  return __builtin_clzll(n);
}

/// @brief Finds the smallest number of bits needed to represent the given
/// value.
int bit_width(unsigned long long x) {
  return std::numeric_limits<unsigned long long>::digits - ::countl_zero(x);
}

int topbit(unsigned long long x) {
  return bit_width(x) - 1;
}

template<typename T> int ceil_log2(T x) {
  if (x <= 1)
    return 0;
  return bit_width(static_cast<unsigned long long>(x - 1));
}

/// @brief Round down to power of 2.
template<typename int_t> inline int_t bit_floor(int_t x) {
  assert(x >= 0);
  while (x > lowbit(x)) {
    x &= x - 1;
  }
  return x;
}

/// @brief Round up to power of 2.
template<typename T> T bit_ceil(T x) {
  if (x <= 1u)
    return T(1);
  return T(1) << bit_width(static_cast<unsigned long long>(x - 1));
}

inline bool is_subset(long long sub, long long s) {
  return (sub & s) == sub;
}

template<typename T> struct subset_tuple {
  explicit subset_tuple(T u) : s(u), u(u) {}
  const T& ref = s;
  T s;
  const T u;
};

#define for_each_nonempty_subset(s, u)                                         \
  for (auto&& [s, JOIN(subset_, __LINE__), JOIN(set_, __LINE__)] =             \
           subset_tuple(u);                                                    \
       s > 0; JOIN(subset_, __LINE__) = (s - 1) & JOIN(set_, __LINE__))

template<typename T> struct loop_controller {
  T loop_var;
  bool flag;
  const T u;
  explicit loop_controller(T u) : loop_var(u), flag(u >= 0), u(u) {}
  void after() {
    flag = loop_var > 0;
    loop_var = (loop_var - 1) & u;
  }
};

#define for_each_subset(s, u)                                                  \
  loop_controller JOIN(loop_controller_, __LINE__)(u);                         \
  for (const auto& s = JOIN(loop_controller_, __LINE__).loop_var;              \
       JOIN(loop_controller_, __LINE__).flag;                                  \
       JOIN(loop_controller_, __LINE__).after())

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BIT_HPP_