//
// Created by zjsdu on 10/12/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BIT_SEGMENT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BIT_SEGMENT_HPP_
#include <set>
#include <array>
#include <cassert>
// A bit_segment represents a bit sequence that supports query about a maximal
// segment of consecutive 1 bits, called a segment for short.
class bit_segment {
  int n_;
  std::set<int> pos0;

 public:
  bit_segment() = default;
  explicit bit_segment(int n, bool init_value) : n_(n), pos0{-1, n_} {
    assert(n_ > 0);
    if (init_value == 0) {
      for (int i = 0; i < n_; i++) {
        pos0.insert(i);
      }
    }
  }

  void init(int n, bool value) {
    assert(pos0.empty());
    assert(n > 0);
    n_ = n;
    pos0.insert(-1);
    pos0.insert(n);
    if (not value) {
      for (int i = 0; i < n; i++) {
        pos0.insert(i);
      }
    }
  }

  // Returns the segment where bit i belongs if it was set.
  std::pair<int, int> virtual_set(int i) const {
    assert(0 <= i and i < n_);
    auto it = pos0.lower_bound(i);
    int l = *std::prev(it);
    int r = *it == i ? *std::next(it) : i;
    return {l + 1, r - 1};
  }

  // Set bit i to true, returns the segment where bit i belongs AFTER it was
  // reset.
  std::pair<int, int> set(int i) {
    assert(0 <= i and i < n_);
    auto it = pos0.find(i);
    assert(it != pos0.end());
    int l = *prev(it);
    int r = *next(it);
    pos0.erase(it);
    return {l + 1, r - 1};
  }

  // Set bit i to false, returns the segment where bit i belongs BEFORE it was
  // reset.
  std::pair<int, int> reset(int i) {
    assert(0 <= i and i < n_);
    auto it = pos0.lower_bound(i);
    assert(*it != i);
    int r = *it;
    int l = *prev(it);
    pos0.insert(i);
    return {l + 1, r - 1};
  }

  // Returns the segment of bit i before or after flip and the value of bit i
  // after flip.
  std::pair<std::pair<int, int>, bool> flip(int i) {
    assert(0 <= i and i < n_);
    return pos0.count(i) ? std::make_pair(set(i), true)
                         : std::make_pair(reset(i), false);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BIT_SEGMENT_HPP_
