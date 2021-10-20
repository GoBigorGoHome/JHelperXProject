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
  /// \anchor (next)
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

  ///@brief Similar to \c next. \n Will not create a new node if the required one
  /// does not exist, returns -1 instead.
  int next_pos(int current_pos, int bit) const {
    return next_[current_pos][bit];
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BITWISE_TRIE_HPP_
