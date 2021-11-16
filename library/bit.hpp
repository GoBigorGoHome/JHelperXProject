#include <macros.h>
#include <cassert>
template<typename int_t> inline int_t lowbit(int_t x) {
  return x & -x;
}

/// @brief Finds the smallest number of bits needed to represent the given
/// value.
inline int bit_width(unsigned long long x) {
  assert(x > 0);
  return int(sizeof(unsigned long long) * 8 - __builtin_clzll(x));
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
unsigned long long bit_ceil(unsigned long long x) {
  if (x == 0)
    return 1;
  return x == lowbit(x) ? x : 1ULL << bit_width(x);
}

template<typename T> inline int bit(T a, int i) {
  return a >> i & 1;
}

inline bool is_subset(long long sub, long long s) {
  return (sub & s) == sub;
}

#define popcnt(x) __builtin_popcountll((x))

template<typename T> struct subset_tuple {
  explicit subset_tuple(T u) : s(u), u(u) {}
  const T &ref = s;
  T s;
  const T u;
};

#define for_each_nonempty_subset(s, u)                                         \
  for (auto &&[s, JOIN(subset_, __LINE__), JOIN(set_, __LINE__)] =             \
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
  for (const auto &s = JOIN(loop_controller_, __LINE__).loop_var;              \
       JOIN(loop_controller_, __LINE__).flag;                                  \
       JOIN(loop_controller_, __LINE__).after())
