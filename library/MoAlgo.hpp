//
// Created by zjs on 2023/3/11.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MOALGO_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MOALGO_HPP_
#include <vector>
#include <cmath>
#include <cassert>
#include <numeric>
#include <algorithm>
// A simple version of Mo's algorithm for range queries.
template<typename T, typename U, typename FuncAdd, typename FuncRemove>
std::vector<U> MoAlgo(const std::vector<T> &a,
                      const std::vector<std::pair<int, int>> &q, FuncAdd add,
                      FuncRemove remove, U &ans) {
  int B = std::sqrt(a.size());
  std::vector<int> I(q.size());
  std::vector<U> res(q.size());
  std::iota(I.begin(), I.end(), 0);
  std::sort(I.begin(), I.end(), [&](int i, int j) {
    int bi = q[i].first / B, bj = q[j].first / B;
    if (bi != bj)
      return bi < bj;
    return q[i].second < q[j].second;
  });
  int l = 0, r = -1;
  for (int i : I) {
    auto [L, R] = q[i];
    while (L < l)
      add(a[--l]);
    while (r < R)
      add(a[++r]);
    while (l < L)
      remove(a[l++]);
    while (R < r)
      remove(a[r--]);
    res[i] = ans;
  }
  return res;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MOALGO_HPP_
