//
// Created by zjsdu on 1/6/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BINOM_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BINOM_HPP_
#include <vector>
#include <cassert>
template<typename T> struct Binom {
  std::vector<T> fact_, inv_fact_;
  int n_;
  explicit Binom(int n) : fact_(n + 1), inv_fact_(n + 1), n_(n) {
    fact_[0] = 1;
    for (int i = 1; i <= n; ++i)
      fact_[i] = fact_[i - 1] * i;
    inv_fact_[n] = 1 / fact_[n];
    for (int i = n; i >= 1; --i)
      inv_fact_[i - 1] = inv_fact_[i] * i;
  }

  T binom(int x, int y) const {
    if (x < 0 || y < 0)
      return 0;
    if (x < y)
      return 0;
    assert(x <= n_);
    return fact_[x] * inv_fact_[y] * inv_fact_[x - y];
  }

  T permutation(int n, int r) const {
    assert(n >= 0 && r >= 0);
    if (r > n)
      return 0;
    assert(n <= n_);
    return fact_[n] * inv_fact_[n - r];
  }

  T fact(int n) const {
    assert(n >= 0 && n <= n_);
    return fact_[n];
  }

  T inv_fact(int n) const {
    assert(n >= 0 && n <= n_);
    return inv_fact_[n];
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BINOM_HPP_
