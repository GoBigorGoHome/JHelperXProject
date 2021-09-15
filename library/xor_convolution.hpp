//
// Created by zjsdu on 9/15/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_CONVOLUTION_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_CONVOLUTION_HPP_
#include <vector>
#include <cassert>

template<typename T> void transform(std::vector<T> &a) {
  int n = int(a.size());
  // The length of a must be a power of 2.
  assert(n > 0 and (n & (n - 1)) == 0);
  for (int len = 1; len < n; len *= 2) {
    for (int i = 0; i < n; i += 2 * len) {
      for (int j = i; j < i + len; ++j) {
        T x = a[j], y = a[j + len];
        a[j] = x + y;
        a[j + len] = x - y;
      }
    }
  }
}

template<typename T> void inv_transform(std::vector<T> &a) {
  int n = int(a.size());
  for (int len = 1; len < n; len *= 2) {
    for (int i = 0; i < n; i += 2 * len) {
      for (int j = i; j < i + len; ++j) {
        T x = a[j], y = a[j + len];
        a[j] = (x + y) / 2;
        a[j + len] = (x - y) / 2;
      }
    }
  }
}

template<typename T>
std::vector<T> xor_convolution(std::vector<T> a, std::vector<T> b) {
  int n = (int) a.size();
  int m = (int) b.size();
  assert(n > 0 and m > 0);
  int t = std::max(n, m);
  int z = 1;
  while (z < t) {
    z *= 2;
  }
  a.resize(z, 0);
  b.resize(z, 0);
  transform(a);
  transform(b);
  for (int i = 0; i < z; i++) {
    a[i] *= b[i];
  }
  inv_transform(a);
  return a;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_CONVOLUTION_HPP_
