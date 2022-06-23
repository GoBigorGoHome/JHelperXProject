//
// Created by zjsdu on 1/31/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MATRIX_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MATRIX_HPP_

#include <vector>
#include <cassert>

template<typename T> using Vec = std::vector<T>;// Vec是列向量。
template<typename T> using Mat = std::vector<Vec<T>>;

template<typename T> Mat<T> Id(int n) {
  Mat<T> res(n, Vec<T>(n));
  for (int i = 0; i < n; i++)
    res[i][i] = 1;
  return res;
}

template<typename T> Mat<T> operator*(const Mat<T>& a, const Mat<T>& b) {
  Mat<T> c(a.size(), Vec<T>(b[0].size()));
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a[0].size(); j++) {
      // optimization for sparse matrix
      if (a[i][j] != 0) {
        for (size_t k = 0; k < b[0].size(); k++) {
          c[i][k] += a[i][j] * b[j][k];
        }
      }
    }
  }
  return c;
}

template<typename T> Vec<T> operator*(const Mat<T>& a, const Vec<T>& b) {
  Vec<T> c(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a[0].size(); j++) {
      c[i] += a[i][j] * b[j];
    }
  }
  return c;
}

template<typename T> Mat<T> operator+(const Mat<T>& a, const Mat<T>& b) {
  Mat<T> c(a.size(), Vec<T>(a[0].size()));
  for (size_t i = 0; i < a.size(); ++i)
    for (size_t j = 0; j < a[i].size(); ++j) {
      c[i][j] = a[i][j] + b[i][j];
    }
  return c;
}

template<typename T> Mat<T> pow(Mat<T> a, long long n) {
  assert(n >= 0);
  Mat<T> res(a.size(), Vec<T>(a.size()));
  for (size_t i = 0; i < a.size(); i++) {
    res[i][i] = 1;
  }
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
