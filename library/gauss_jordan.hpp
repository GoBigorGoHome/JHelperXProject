//
// Created by zjs on 2022/1/2.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GAUSS_JORDAN_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GAUSS_JORDAN_HPP_

// Gauss-Jordan elimination

#include <vector>
#include <cassert>

template<typename F>
/// @param n_eq number of equations
std::vector<int> gauss_jordan(std::vector<std::vector<F>>& a, int n_var) {
  assert(n_var > 0);
  int n_eq = static_cast<int>(a.size());
  assert(n_eq > 0);
  int w = static_cast<int>(a[0].size());
  assert(n_var <= w);
  int rank = 0;
  std::vector<int> leading_cols;
  const F zero = F(0);

  for (int c = 0; c < n_var; c++) {
    // select a pivoting row
    int pivot = rank;
    while (pivot < n_eq && a[pivot][c] == zero)
      ++pivot;
    if (pivot == n_eq)
      continue;

    std::swap(a[rank], a[pivot]);
    leading_cols.push_back(c);

    F inv_a = 1 / a[rank][c];
    std::vector<int> nonzero_cols;
    for (int j = c; j < w; j++)
      if (a[rank][j] != zero) {
        nonzero_cols.push_back(j);
        a[rank][j] *= inv_a;
      }

    for (int r = 0; r < n_eq; r++) {
      if (r == rank || a[r][c] == 0)
        continue;
      F coeff = -a[r][c];
      for (int j : nonzero_cols)
        a[r][j] += coeff * a[rank][j];
    }
    rank++;
  }
  // 至此已化成行最简形。
  return leading_cols;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GAUSS_JORDAN_HPP_
