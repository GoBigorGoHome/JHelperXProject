//
// Created by zjsdu on 2022/2/19.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_POINT_SEGTREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_POINT_SEGTREE_HPP_

#include <vector>
#include <cassert>
#include <bit.hpp>

/// @brief 支持单点查询，区间修改，不支持区间查询的线段树。
/// @tparam S 序列的元素类型，不要求 S 里有单位元。
/// @tparam op S 上满足结合律的一种运算。
template<class S, S (*op)(S, S)> struct point_segtree {
 public:
  point_segtree(int n, S init_value)
      : _n(n), log(ceil_log2(_n)), size(1 << log), d(_n, init_value),
        lz(size, {S{}, false}) {
    assert(_n > 0);
  }

  explicit point_segtree(const std::vector<S>& v)
      : point_segtree(int(v.size), S{}) {
    for (int i = 0; i < _n; i++)
      d[i] = v[i];
  }

  void set(int p, S x) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    d[p - size] = x;
  }

  S get(int p) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    return d[p - size];
  }

  void apply(int p, S s) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    d[p - size] = op(s, d[p - size]);
  }

  /// @brief 区间 [l, r) 左闭右开。
  void apply(int l, int r, S s) {
    assert(0 <= l && l <= r && r <= _n);
    if (l == r)
      return;

    l += size;
    r += size;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    while (l < r) {
      if (l & 1)
        all_apply(l++, s);
      if (r & 1)
        all_apply(--r, s);
      l >>= 1;
      r >>= 1;
    }
  }

 private:
  int _n, log, size;
  std::vector<S> d;
  std::vector<std::pair<S, bool>> lz;

  void all_apply(int k, S s) {
    if (k < size) {
      if (lz[k].second)
        lz[k].first = op(s, lz[k].first);
      else
        lz[k] = {s, true};
    } else if (k - size < _n)
      d[k - size] = op(s, d[k - size]);
  }

  void push(int k) {
    if (lz[k].second) {
      all_apply(2 * k, lz[k].first);
      all_apply(2 * k + 1, lz[k].first);
      lz[k].second = false;
    }
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_POINT_SEGTREE_HPP_
