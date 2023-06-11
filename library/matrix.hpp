//
// Created by zjsdu on 1/31/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MATRIX_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MATRIX_HPP_

#include <vector>
#include <cassert>

template<typename T> using vec = std::vector<T>;

template<typename T> class mat : public std::vector<std::vector<T>> {
 public:
  template<typename Int>
  mat(Int n, Int m) : std::vector<std::vector<T>>(n, std::vector<T>(m)) {}
  template<typename Int> mat(Int n) : mat(n, n) {}
};

template<typename T, typename Int> mat<T> ID(Int n) {
  mat<T> res(n);
  for (Int i = 0; i < n; i++)
    res[i][i] = 1;
  return res;
}

template<typename T> mat<T> operator*(const mat<T> &a, const mat<T> &b) {
  assert(b.size() > 0);
  mat<T> c(a.size(), b[0].size());
  for (std::size_t i = 0; i < a.size(); i++) {
    for (std::size_t j = 0; j < a[0].size(); j++) {
      // optimization for sparse matrix
      if (a[i][j] != 0) {
        for (std::size_t k = 0; k < b[0].size(); k++) {
          c[i][k] += a[i][j] * b[j][k];
        }
      }
    }
  }
  return c;
}

template<typename T> vec<T> operator*(const mat<T> &a, const vec<T> &b) {
  vec<T> c(a.size());
  for (std::size_t i = 0; i < a.size(); i++) {
    for (std::size_t j = 0; j < a[0].size(); j++) {
      c[i] += a[i][j] * b[j];
    }
  }
  return c;
}

template<typename T> mat<T> operator+(const mat<T> &a, const mat<T> &b) {
  assert(a.size() > 0);
  mat<T> c(a.size(), a[0].size());
  for (std::size_t i = 0; i < a.size(); ++i)
    for (std::size_t j = 0; j < a[i].size(); ++j) {
      c[i][j] = a[i][j] + b[i][j];
    }
  return c;
}

template<typename T> mat<T> power(mat<T> a, long long n) {
  assert(n >= 0);
  mat<T> res = ID<T>(a.size());
  while (n) {
    if (n & 1) {
      res = res * a;
    }
    a = a * a;
    n >>= 1;
  }
  return res;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MATRIX_HPP_
