//
// Created by Lenovo on 2025/5/17.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_HPP_
#include <vector>
#include <cassert>
#include <bit>

template <class S, S (*op)(S, S), S (*e)()>
class binary_trie {
  struct node {
    node* ch[2];
    S data;
  };
  int w_;
  int sz; // number of nodes;
  std::vector<node> pool;
  node* new_node() {
    pool[sz] = {{nullptr, nullptr}, e()};
    return pool.data() + (sz++);
  }



 public:
  binary_trie(int n, int w) :  w_(w), sz(0) {
    assert(w > 0);
    assert(n > 0);
    int len = std::bit_width((unsigned int) n) - 1;
    int max_sz;
    if (w < len)
      max_sz = 1 << (w + 1);
    else
      max_sz =  (1 << (len + 1)) + (w - len) * n;
    pool.assign(max_sz, node{});
    new_node(); // create root
  }

  const node* insert(int key, S val) {
    node* ptr = pool.data();
    ptr->data = op(ptr->data, val);
    for (int i = w_ - 1; i >= 0; i--) {
      int b = key >> i & 1;
      if (!(ptr->ch[b])) {
        ptr->ch[b] = new_node();
      }
      ptr = ptr->ch[b];
      ptr->data = op(ptr->data, val);
    }
    return ptr;
  }

  const node* root() const {
    return pool.data();
  }

  void clear() {
    sz = 0;
    new_node();
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_HPP_
