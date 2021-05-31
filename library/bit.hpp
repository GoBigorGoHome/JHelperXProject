#include <macros.h>
#include <cassert>
template<typename int_t> inline int_t lowbit(int_t x) {
  return x & -x;
}

inline int h_bit(unsigned long long x) {
  return int(sizeof(unsigned long long) * 8 - __builtin_clzll(x));
}

unsigned long long pow2(unsigned long long x) {// round up to power of 2
  if (x == 0)
    return 1;
  return x == lowbit(x) ? x : 1ull << h_bit(x);
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
