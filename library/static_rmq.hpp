//
// Created by zjs on 2022/1/4.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STATIC_RMQ_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STATIC_RMQ_HPP_

#include <monoid_segtree.hpp>
#include <limits>

template<typename T> class static_range_min {
  static T Min(T a, T b) { return std::min(a, b); }
  monoid_segtree<T, std::numeric_limits<T>::max(), Min> tree;
  int n;

 public:
  explicit static_range_min(const std::vector<T>& array)
      : tree(array), n(static_cast<int>(array.size())) {}

  T get(int l, int r) const {
    if (l > r)
      return std::numeric_limits<T>::max();
    assert(0 <= l and r < n);
    return tree.product(l, r);
  }

  T get() const { return tree.all_product(); }
};

template<typename T> class static_range_max {
  static T Max(T a, T b) { return std::max(a, b); }
  monoid_segtree<T, std::numeric_limits<T>::min(), Max> tree;
  int n;

 public:
  explicit static_range_max(const std::vector<T>& array)
      : tree(array), n(static_cast<int>(array.size())) {}

  T get(int l, int r) const {
    if (l > r)
      return std::numeric_limits<T>::min();
    assert(0 <= l and r < n);
    return tree.product(l, r);
  }

  T get() const { return tree.all_product(); }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STATIC_RMQ_HPP_
