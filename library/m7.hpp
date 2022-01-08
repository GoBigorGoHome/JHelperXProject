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
mint C(int n, int m) {
  if (m < 0 or m > n)
    return 0;
  while (n + 1 > (int) factorial.size()) {
    factorial.push_back(factorial.back() * (int) factorial.size());
  }
  while (n + 1 > (int) inv_factorial.size()) {
    inv_factorial.push_back(inv_factorial.back() / (int) inv_factorial.size());
  }
  return factorial[n] * inv_factorial[m] * inv_factorial[n - m];
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_M7_HPP_
