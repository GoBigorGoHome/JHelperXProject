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

/// @tparam F type of field.
/// @brief f is a polynomial of degree d on field F. Given f(0), f(1),
/// ..., f(d), and x, compute f(x) in O(d) time using Lagrange interpolation.
/// @param values list of values f(0), f(1), ..., f(d).
/// @param x the evaluation point.
template<typename F>
F lagrange_interpolation(const std::vector<F>& values, F x) {
  assert(values.size() > 0);
  int n = (int) values.size();

  std::vector<F> inv_factorial(n);
  F factorial = 1;
  for (int i = 2; i < n; i++)
    factorial *= i;
  inv_factorial[n - 1] = F(1) / factorial;
  for (int i = n - 2; i >= 0; i--)
    inv_factorial[i] = inv_factorial[i + 1] * (i + 1);

  std::vector<F> r_prod(n);
  F t = 1;
  for (int i = n - 1; i >= 0; i--) {
    r_prod[i] = t;
    t *= x - i;
  }
  F l_prod = 1;
  F ans = 0;
  for (int i = 0; i < n; i++) {
    if ((n - 1 - i) & 1)
      ans -= values[i] * l_prod * r_prod[i] * inv_factorial[i]
          * inv_factorial[n - 1 - i];
    else
      ans += values[i] * l_prod * r_prod[i] * inv_factorial[i]
          * inv_factorial[n - 1 - i];
    l_prod *= x - i;
  }
  return ans;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_HPP_
