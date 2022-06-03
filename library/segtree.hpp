//
// Created by zjsdu on 7/7/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SEGTREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SEGTREE_HPP_

#include <vector>
#include <cassert>
#include <bit.hpp>

/// @brief A monoid segment tree implemented as a complete binary tree.
/// \tparam T Type of elements in the monoid.
/// \tparam e Identity of the monoid.
/// \tparam Op Binary operation of the monoid. Lambda expressions not allowed.
/// See https://stackoverflow.com/q/5849059/6793559.
template<typename T, T (*e)(), T (*Op)(T, T)> class segtree {
 public:
  explicit segtree(int n) : n_(n) {
    log = ceil_log2(n_);
    size = 1 << log;
    data.assign(size * 2, e());
    reset_.assign(size, false);
  }

  template<typename U>
  explicit segtree(const std::vector<U>& v) : segtree((int) v.size()) {
    for (int i = 0; i < n_; i++) {
      data[i + size] = v[i];
    }
    for (int i = size - 1; i > 0; --i) {
      update(i);
    }
  }

  T get(int position) {
    assert(0 <= position and position < n_);
    position += size;
    for (int i = log; i >= 1; i--)
      push(position >> i);
    return data[position];
  }

  void set(int position, T value) {
    assert(0 <= position and position < n_);
    position += size;
    for (int i = log; i >= 1; i--)
      push(position >> i);
    data[position] = value;
    for (int i = 1; i <= log; i++)
      update(position >> i);
  }

  // Generally, assume Op is non-commutative.
  T product(int l, int r) {
    if (l > r)
      return e();
    assert(0 <= l and r < n_);
    l += size;
    r += size + 1;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    T product_l = e(), product_r = e();
    while (l < r) {
      if (l & 1)
        product_l = Op(product_l, data[l++]);
      if (r & 1)
        product_r = Op(data[--r], product_r);
      l /= 2;
      r /= 2;
    }
    return Op(product_l, product_r);
  }

  T all_product() const { return data[1]; }

  /// Set S[l..r] to Identity.
  void reset(int l, int r) {
    if (l > r)
      return;
    assert(0 <= l and r < n_);
    l += size;
    r += size + 1;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1)
          all_apply(l++);
        if (r & 1)
          all_apply(--r);
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }

    for (int i = 1; i <= log; i++) {
      if (((l >> i) << i) != l)
        update(l >> i);
      if (((r >> i) << i) != r)
        update((r - 1) >> i);
    }
  }

 private:
  std::vector<T> data;
  std::vector<bool> reset_;
  int size;// number of leaves
  int n_;  // number of data items.
  int log;

  void update(int index) {
    data[index] = Op(data[index * 2], data[index * 2 + 1]);
  }

  void all_apply(int k) {
    data[k] = e();
    if (k < size)
      reset_[k] = true;
  }

  void push(int k) {
    if (reset_[k]) {
      all_apply(2 * k);
      all_apply(2 * k + 1);
      reset_[k] = false;
    }
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SEGTREE_HPP_
