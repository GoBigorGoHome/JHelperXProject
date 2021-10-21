//
// Created by zjs on 2021/10/20.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_HPP_
#include <vector>
#include <array>
#include <cassert>
template<typename T> class bitwise_trie {
  std::vector<T> data_;
  std::vector<std::array<int, 2>> next_;

 public:
  bitwise_trie() {
    data_.push_back({});
    next_.push_back({-1, -1});
  }

  T *data(int pos) {
    assert(0 <= pos and pos < (int) data_.size());
    return &data_[pos];
  }

  /// @brief Returns the next position going from \p pos by character \p bit. If
  /// the next position does not exist, create a new node.
  /// @param bit is 0 or 1.
  int next(int pos, int bit) {
    assert(0 <= pos and pos < (int) data_.size());
    if (next_[pos][bit] == -1) {
      next_[pos][bit] = (int) data_.size();
      data_.push_back({});
      next_.push_back({-1, -1});
    }
    return next_[pos][bit];
  }

  /// @brief Similar to \c next. \n Will not create a new node if the required
  /// one does not exist, returns -1 instead.
  int next_pos(int current_pos, int bit) const {
    return next_[current_pos][bit];
  }

  /// @brief Insert a bit sequence of length \p h + 1 represented as an integer
  /// \p key, starting at \p h-th bit of \p key.
  template<typename Int> int insert(Int key, int h) {
    int pos = 0;
    for (int i = h; i >= 0; i--) {
      pos = next(pos, (int) (key >> i & 1));
    }
    return pos;
  }

  /// @brief Find the minimum value of \p x XOR \em key over all keys in the
  /// trie. All keys in the trie should have the same length \p h + 1.
  template<typename Int> Int min_xor(Int x, int h) const {
    Int result = 0;
    int pos = 0;
    for (int i = h; i >= 0; i--) {
      int bit = (int) (x >> i & 1);
      if (int t = next_pos(pos, bit); t != -1) {
        pos = t;
      } else {
        pos = next_pos(pos, 1 - bit);
        if (pos == -1)
          return -1;
        result |= (Int) 1 << i;
      }
    }
    return result;
  }

  /// @brief Find the maximum value of \p x XOR \em key over all keys in the
  /// trie. All keys in the trie should have the same length \p h + 1.
  template<typename Int> Int max_xor(Int x, int h) const {
    Int result = 0;
    int pos = 0;
    for (int i = h; i >= 0; i--) {
      int bit = (int) (x >> i & 1);
      if (int t = next(pos, 1 - bit); t != -1) {
        pos = t;
        result |= (Int) 1 << i;
      } else {
        pos = next_pos(pos, bit);
        if (pos == -1)
          return -1;
      }
    }
    return result;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_HPP_
