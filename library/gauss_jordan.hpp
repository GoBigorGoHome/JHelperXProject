//
// Created by zjs on 2022/1/2.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GAUSS_JORDAN_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GAUSS_JORDAN_HPP_

// Gauss-Jordan elimination
// 当有无穷多组解时任意输出一组解。
#include <vector>
#include <cassert>

template<typename F, typename Mat>
/// @param n_eq number of equations
/// @param n_var number of variables
std::vector<F> gauss_jordan(Mat a, int n_eq, int n_var) {
  assert(n_eq > 0);
  assert(n_var > 0);
  int rank = 0;
  std::vector<int> leading_cols;
  const F zero = F(0);

  for (int i = 0; i < n_var; i++) {
    // select a pivoting row
    int pivot = rank;
    while (pivot < n_eq && a[pivot][i] == zero)
      ++pivot;
    if (pivot == n_eq)
      continue;

    std::swap(a[rank], a[pivot]);
    leading_cols.push_back(i);

    std::vector<int> nonzero_cols;
    for (int j = i + 1; j <= n_var; j++)
      if (a[rank][j] != zero)
        nonzero_cols.push_back(j);

    for (int j : nonzero_cols)
      a[rank][j] /= a[rank][i];

    for (int row = rank + 1; row < n_eq; row++) {
      if (a[row][i] == zero)// This is an important optimization.
        continue;
      for (int col : nonzero_cols) {
        a[row][col] -= a[row][i] * a[rank][col];
      }
    }
    rank++;
  }

  for (int row = rank; row < n_eq; row++) {
    if (a[row][n_var] != zero)
      return {};// No solution.
  }

  // set undetermined variables zero.
  std::vector<F> sol(n_var);
  for (int i = rank - 1; i >= 0; i--) {
    F t = a[i][n_var];
    for (int col = n_var - 1; col > leading_cols[i]; col--) {
      t -= a[i][col] * sol[col];
    }
    sol[leading_cols[i]] = t;
  }
  return sol;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GAUSS_JORDAN_HPP_
