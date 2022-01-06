//
// Created by zjs on 2021/10/20.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_MULTISET_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_MULTISET_HPP_

#include <vector>
#include <array>
#include <cassert>

template<int bit_width> class bitwise_trie_multiset {
  static_assert(bit_width > 0);
  std::vector<std::array<int, 2>> next_;
  std::vector<int> count_;

 public:
  bitwise_trie_multiset() : next_(1, {-1, -1}), count_(1, 0) {}

  /// @brief Returns the next position going from \p pos by character \p bit. If
  /// the next position does not exist, create a new node.
  /// @param bit is 0 or 1.
  int next(int pos, int bit) {
    assert(0 <= pos and pos < (int) next_.size());
    assert(bit == 0 or bit == 1);
    if (next_[pos][bit] == -1) {
      next_[pos][bit] = (int) next_.size();
      next_.push_back({-1, -1});
      count_.push_back(0);
    }
    return next_[pos][bit];
  }

  /// @brief Similar to \c next. \n Will not create a new node if the required
  /// one does not exist, returns -1 instead.
  int next_pos(int current_pos, int bit) const {
    assert(bit == 0 or bit == 1);
    return next_[current_pos][bit];
  }

  /// @brief Insert a bit sequence of length bit_width represented as an
  /// integer \p key, starting from the (bit_width - 1)-th bit of \p key.
  template<typename Int> int insert(Int key) {
    int pos = 0;
    count_[pos]++;
    for (int i = bit_width - 1; i >= 0; i--) {
      pos = next(pos, (int) (key >> i & 1));
      count_[pos]++;
    }
    return pos;
  }

  template<typename Array, typename Predicate>
  int insert(const Array& array, Predicate is_zero) {
    assert(std::size(array) >= bit_width);
    int pos = 0;
    count_[pos]++;
    for (int i = bit_width - 1; i >= 0; i--) {
      pos = next(pos, bool(is_zero(array[i])));
      count_[pos]++;
    }
    return pos;
  }

  /// @brief Find the minimum value of \p x XOR \em key over all keys in the
  /// trie.
  template<typename Int> Int min_xor(Int x) const {
    assert(not this->empty());
    Int result = 0;
    int pos = 0;
    for (int i = bit_width - 1; i >= 0; i--) {
      int bit = (int) (x >> i & 1);
      if (int t = next_pos(pos, bit); t != -1) {
        pos = t;
      } else {
        pos = next_pos(pos, 1 - bit);
        result |= (Int) 1 << i;
      }
    }
    return result;
  }

  /// @brief Find the maximum value of \p x XOR \em key over all keys in the
  /// trie.
  template<typename Int> Int max_xor(Int x) const {
    assert(not this->empty());
    Int result = 0;
    int pos = 0;
    for (int i = bit_width - 1; i >= 0; i--) {
      int bit = (int) (x >> i & 1);
      if (int t = next_pos(pos, 1 - bit); t != -1) {
        pos = t;
        result |= (Int) 1 << i;
      } else {
        pos = next_pos(pos, bit);
      }
    }
    return result;
  }

  int count(int pos) const {
    assert(0 <= pos and pos < (int) next_.size());
    return count_[pos];
  }

  void clear() {
    next_.assign(1, {-1, -1});
    count_.assign(1, 0);
  }

  bool empty() const { return next_.size() <= 1; }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_MULTISET_HPP_
