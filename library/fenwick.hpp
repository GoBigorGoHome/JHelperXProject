#include <algorithm>
#include <cassert>
#include <vector>
template<typename T>
class BIT {
  // 0-indexed
  std::vector<T> a;
  int size, pow2;

 public:
  explicit BIT(int n) {
    assert(n > 0);
    a.assign(n, 0);
    size = n;
    // round down to power of 2
    pow2 = 1;
    while (pow2 <= n / 2) { pow2 *= 2; }
  }

  T sum(int x) const {
    T res = 0;
    while (x >= 0) {
      res += a[x];
      x = (x & (x + 1)) - 1;
    }
    return res;
  }

  void add(int x, T v) {
    while (x < size) {
      a[x] += v;
      x |= (x + 1);
    }
  }

  int lower_bound(T s) const {
    int pos = 0;
    for (int len = pow2; len >= 1; len /= 2) {
      if (pos + len - 1 < size && a[pos + len - 1] < s) {
        s -= a[pos + len - 1];
        pos += len;
      }
    }
    return pos;
  }

  void clear() { std::fill(a.begin(), a.end(), 0); }
};
