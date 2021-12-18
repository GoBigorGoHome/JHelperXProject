//
// Created by zjsdu on 2021/12/15.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_HPP_

#include <vector>
#include <cassert>

template<typename T> using Poly = std::vector<T>;

template<typename T> Poly<T> operator*(const Poly<T>& a, const Poly<T>& b) {
  assert(a.size() > 0);
  assert(b.size() > 0);
  Poly<T> c(a.size() + b.size() - 1);
  auto n = a.size(), m = b.size();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      c[i + j] += a[i] * b[j];
  return c;
}

template<typename T> Poly<T>& operator*=(Poly<T>& a, const Poly<T>& b) {
  assert(a.size() > 0);
  assert(b.size() > 0);
  int n = (int) a.size(), m = (int) b.size();
  a.resize(n + m - 1);
  for (int i = n - 1; i >= 0; i--) {
    for (int j = 1; j < m; j++)
      a[i + j] += a[i] * b[j];
    a[i] *= b[0];
  }
  return a;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_HPP_
