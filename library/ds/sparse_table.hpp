//
// Created by zjs on 4/25/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_SPARSE_TABLE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_SPARSE_TABLE_HPP_

#include <vector>
#include <cassert>

template<typename T, typename F>
class sparse_table {
 public:
  int n;
  std::vector<std::vector<T>> mat;
  F func;
  T e;

  /// @param F T上的二元运算，即 (T, T) -> T。F应该是幂等的和交换的，即有 F(x,x)
  /// = x 和 F(x,y) = F(y,x)。
  /// @param 运算 F 下的单位元。
  sparse_table(const std::vector<T> &a, const F &f, T e) : func(f), e(e) {
    n = static_cast<int>(a.size());
    int max_log = 32 - __builtin_clz(n);
    mat.resize(max_log);
    mat[0] = a;
    for (int j = 1; j < max_log; j++) {
      mat[j].resize(n - (1 << j) + 1);
      for (int i = 0; i <= n - (1 << j); i++) {
        mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
      }
    }
  }

  T get(int from, int to) const {
    if (from > to)
      return e;
    assert(0 <= from && from <= to && to <= n - 1);
    int lg = 32 - __builtin_clz(to - from + 1) - 1;
    return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_SPARSE_TABLE_HPP_
