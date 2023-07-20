//
// Created by zjs on 7/1/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_MAP_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_MAP_HPP_

#include <vector>
#include <array>
#include <cassert>

template<int bit_width, int NODES, typename Value> class binary_trie_map {
  static_assert(bit_width > 0);
  static_assert(NODES > 0);
  struct Node {
    Node *l = nullptr, *r = nullptr;
    int cnt = 0;
    Value val;
  };
  Node *pool;
  int pid;

 public:
  using np = Node *;

  np root() const { return pool; }
  binary_trie_map() : pid(1) { pool = new Node[NODES]; }

  ~binary_trie_map() { delete[] pool; }

  /// @brief Insert a bit sequence of length bit_width represented as an
  /// integer \p key, starting from the (bit_width - 1)-th bit of \p key.
  template<typename Int, typename Func> void insert(Int key, Func update) {
    return rec_insert(pool, key, bit_width - 1, update);
  }

  template<typename Int, typename Func> void erase(Int key, Func update) {
    return rec_erase(pool, key, bit_width - 1, update);
  }

  /// @brief Find the minimum value of \p x XOR \em key over all keys in the
  /// trie.
  template<typename Int> Int min_xor(Int x) const {
    assert(not this->empty());
    Int result = 0;
    np ptr = pool;
    for (int i = bit_width - 1; i >= 0; i--) {
      int bit = (int) (x >> i & 1);
      if (int t = next_pos(ptr, bit); t) {
        ptr = t;
      } else {
        ptr = next_pos(ptr, 1 - bit);
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
    np ptr = pool;
    for (int i = bit_width - 1; i >= 0; i--) {
      int bit = (int) (x >> i & 1);
      if (np t = next_pos(ptr, 1 - bit); t) {
        ptr = t;
        result |= (Int) 1 << i;
      } else {
        ptr = next_pos(ptr, bit);
      }
    }
    return result;
  }

  int count(np pos) const { return pos->cnt; }

  void clear() {
    pid = 1;
    pool[0] = {};
  }

  bool empty() const { return pool->cnt > 0; }

 private:
  /// @brief Returns the next position going from \p pos by character \p bit. If
  /// the next position does not exist, create a new node.
  /// @param bit is 0 or 1.
  np next(np ptr, int bit) {
    assert(bit == 0 or bit == 1);
    np &ch = bit ? ptr->r : ptr->l;
    if (!ch) {
      ch = &(pool[pid++]);
    }
    return ch;
  }

  /// @brief Similar to \c next. \n Will not create a new node if the required
  /// one does not exist, returns -1 instead.
  np next_pos(np ptr, int bit) const {
    assert(bit == 0 or bit == 1);
    assert(ptr);
    return bit ? ptr->r : ptr->l;
  }

  template<typename Int, typename Func>
  void rec_insert(np root, Int key, int i, Func update) {
    assert(root);
    root->cnt++;
    if (i >= 0)
      rec_insert(next(root, key >> i & 1), key, i - 1, update);
    update(root, key, i);
  }

  template<typename Int, typename Func>
  void rec_erase(np root, Int key, int i, Func update) {
    root->cnt--;
    if (i >= 0)
      rec_erase(next(root, key >> i & 1), key, i - 1, update);
    update(root, key, i);
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_MAP_HPP_
