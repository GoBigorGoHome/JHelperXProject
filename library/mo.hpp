//
// Created by zjs on 3/10/25.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_HPP_

#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

std::vector<int> get_mo_order(const std::vector<std::pair<int, int>> &q) {
  int Q = (int) q.size();
  if (Q == 0)
    return {};
  int N = 1;// max coordinates
  for (auto [x_, y_] : q)
    N = std::max({N, x_, y_});
  int B = std::sqrt(3) * N / sqrt(2 * Q);// from maspy
  B = std::max(B, 1);
  std::vector<int> I(Q);
  std::iota(I.begin(), I.end(), 0);
  sort(I.begin(), I.end(), [&](int a, int b) {
    int aa = q[a].first / B, bb = q[b].first / B;
    if (aa != bb)
      return aa < bb;
    return (aa & 1) ? q[a].second > q[b].second : q[a].second < q[b].second;
  });

  auto cost = [&](int a, int b) -> int {
    return abs(q[I[a]].first - q[I[b]].first)
        + abs(q[I[a]].second - q[I[b]].second);
  };

  // ランダムケースで数パーセント
  for (int k = 0; k < Q - 5; k++) {
    if (cost(k, k + 2) + cost(k + 1, k + 3)
        < cost(k, k + 1) + cost(k + 2, k + 3)) {
      std::swap(I[k + 1], I[k + 2]);
    }
    if (cost(k, k + 3) + cost(k + 1, k + 4)
        < cost(k, k + 1) + cost(k + 3, k + 4)) {
      std::swap(I[k + 1], I[k + 3]);
    }
  }
  return I;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_HPP_
