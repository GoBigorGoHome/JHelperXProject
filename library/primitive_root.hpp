//
// Created by zjsdu on 8/13/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_PRIMITIVE_ROOT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_PRIMITIVE_ROOT_HPP_
#include <vector>
int powmod(long long a, long long b, int p) {
  long long res = 1;
  for (; b; a = a * a % p, b >>= 1)
    if (b & 1)
      res = res * a % p;
  return (int) res;
}
/// return minimum primitive root of modulo p
int generator(int p) {
  std::vector<int> fact;
  int phi = p - 1, n = phi;
  for (int i = 2; i * i <= n; ++i)
    if (n % i == 0) {
      fact.push_back(phi / i);
      do {
        n /= i;
      } while (n % i == 0);
    }
  if (n > 1)
    fact.push_back(phi / n);

  for (int res = 2; res <= p; ++res) {
    bool ok = true;
    for (int f : fact) {
      ok &= powmod(res, f, p) != 1;
      if (not ok)
        break;
    }
    if (ok)
      return res;
  }
  return -1;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_PRIMITIVE_ROOT_HPP_
