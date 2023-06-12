//
// Created by zjsdu on 2022/1/8.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_M7_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_M7_HPP_

#include <vector>
#include <modular.hpp>

using mint = Mint<1000000007>;
mint operator""_m(unsigned long long v) {
  return mint(static_cast<int>(v));
}

std::vector<mint> factorial(1, 1);
std::vector<mint> inv_factorial(1, 1);

mint fact(int n) {
  assert(n >= 0);
  while (n + 1 > (int) factorial.size()) {
    factorial.push_back(factorial.back() * (int) factorial.size());
  }
  return factorial[n];
}

mint inv_fact(int n) {
  assert(n >= 0);
  while (n + 1 > (int) inv_factorial.size()) {
    inv_factorial.push_back(inv_factorial.back() / (int) inv_factorial.size());
  }
  return inv_factorial[n];
}

mint C(int n, int m) {// combination
  if (m < 0 or m > n)
    return 0;
  return fact(n) * inv_fact(m) * inv_fact(n - m);
};

mint P(int n, int m) {// permutation
  if (m < 0 or m > n)
    return 0;
  return fact(n) * inv_fact(n - m);
}

// Number of combinations with repetition.
mint C_rep(int n, int m) {
  if (n < 0 || m < 0)
    return 0;
  if (n == 0)
    return (int) (m == 0);
  return C(n + m - 1, m);
}

// Number of ways to distribute N indistinguishable balls into M bins.
mint distribute(int N, int M) {
  if (N < 0 || M < 0)
    return 0;
  if (M == 0)
    return (int) (N == 0);
  return C(N + M - 1, M - 1);
}

template<typename Int> mint power(mint x, Int n) {
  return Power(x, n);
}

template<int N> struct Pow {
  std::vector<mint> p{1};
  mint operator()(int n) {
    assert(n >= 0);
    while (n + 1 > (int) p.size()) {
      p.push_back(N * p.back());
    }
    return p[n];
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_M7_HPP_
