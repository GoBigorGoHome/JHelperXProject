#include <cassert>
template <typename int_t> inline int_t lowbit(int_t x) { return x & -x; }

inline int h_bit(unsigned long long x) {
  return int(sizeof(unsigned long long) * 8 - __builtin_clzll(x));
}

unsigned long long pow2(unsigned long long x) { // round up to power of 2
  if (x == 0) return 1;
  return x == lowbit(x) ? x : 1ull << h_bit(x);
}

template <typename T> int bit(T a, int i) { return a >> i & 1; }
template <typename T> T get_mid(T l, T r) {
  assert(l <= r);
  return l + (r - l >> 1);
}

#define popcnt(x) __builtin_popcountll((x))