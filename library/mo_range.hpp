//
// Created by zjsdu on 2022/3/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_RANGE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_RANGE_HPP_

#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <cmath>

// copy from https://github.com/maspypy/library/blob/main/ds/mo.hpp

/// Mo's algorithm for static range queries.
struct Mo {
  std::vector<std::pair<int, int>> lr;

  /// add half-open range [l, r)
  void add(int l, int r) { /* [l, r) */
    assert(0 <= l and l < r);
    lr.emplace_back(l, r);
  }

  template<typename AL, typename AR, typename EL, typename ER, typename O>
  void calc(const AL& add_left, const AR& add_right, const EL& erase_left,
            const ER& erase_right, const O& out) {
    int n = 1;
    for (auto [l, r] : lr)
      n = std::max(n, r);
    int q = (int) lr.size();
    int bs = n / std::min<int>(n, std::sqrt(q));// bs: block size
    std::vector<int> ord(q);
    std::iota(std::begin(ord), std::end(ord), 0);
    std::sort(std::begin(ord), std::end(ord), [&](int a, int b) {
      int ablock = lr[a].first / bs, bblock = lr[b].first / bs;
      if (ablock != bblock)
        return ablock < bblock;
      return (ablock & 1) ? lr[a].second > lr[b].second
                          : lr[a].second < lr[b].second;
    });
    int l = 0, r = 0;
    for (int idx : ord) {
      while (l > lr[idx].first)
        add_left(--l);
      while (r < lr[idx].second)
        add_right(r++);
      while (l < lr[idx].first)
        erase_left(l++);
      while (r > lr[idx].second)
        erase_right(--r);
      out(idx);
    }
  }

  template<typename A, typename E, typename O>
  void calc(const A& add, const E& erase, const O& out) {
    calc(add, add, erase, erase, out);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_RANGE_HPP_
