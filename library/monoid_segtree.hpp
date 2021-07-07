//
// Created by zjsdu on 7/7/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MONOID_SEGMENT_TREE_HPP_
#include <vector>
#include <cassert>
/// @brief A monoid segment tree implemented as a complete binary tree.
/// \tparam T Type of elements in the monoid.
/// \tparam Identity Identity of the monoid.
/// \tparam Op Binary operation of the monoid. Lambda expressions not allowed.
/// See https://stackoverflow.com/q/5849059/6793559.
template<typename T, T Identity, T (*Op)(T, T)> class monoid_segtree {
 public:
  explicit monoid_segtree(int n) : n_(n) {
    size = 1;
    while (size < n) {
      size *= 2;
    }
    data.assign(size * 2, Identity);
  }

  template<typename U>
  explicit monoid_segtree(const std::vector<U> &v)
      : monoid_segtree((int) v.size()) {
    for (int i = 0; i < n_; i++) {
      data[i + size] = v[i];
    }
    for (int i = size - 1; i > 0; --i) {
      update(i);
    }
  }

  T get(int position) const {
    assert(0 <= position and position < n_);
    return data[position + size];
  }

  void set(int position, T value) {
    assert(0 <= position and position < n_);
    position += size;
    data[position] = value;
    for (position /= 2; position > 0; position /= 2) {
      update(position);
    }
  }

  // Generally, assume Op is non-commutative.
  T product(int l, int r) const {
    if (l > r)
      return Identity;
    assert(0 <= l and r < n_);
    r += 1;
    T product_l = Identity, product_r = Identity;
    l += size;
    r += size;

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

 private:
  std::vector<T> data;
  int size;// number of leaves
  int n_;  // number of data items.
  void update(int index) {
    data[index] = Op(data[index * 2], data[index * 2 + 1]);
  }
};
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MONOID_SEGMENT_TREE_HPP_

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MONOID_SEGMENT_TREE_HPP_
