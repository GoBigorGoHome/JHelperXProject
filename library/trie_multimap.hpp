//
// Created by zjs on 9/12/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TRIE_MULTIMAP_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TRIE_MULTIMAP_HPP_
#include <array>
#include <vector>
#include <string>
#include <cassert>
template<int sigma_size, typename Value> class trie_multimap {
  static_assert(sigma_size > 0);
  struct Node {
    int parent;
    int key_cnt = 0;// 插入的字符串有多少个是当前节点对应的字符串。
    std::array<int, sigma_size> pos;
    std::vector<Value> vals;
    Node(int parent) : parent(parent) {
      for (int i = 0; i < sigma_size; i++)
        pos[i] = -1;
    }
  };
  std::vector<Node> tree;

 public:
  trie_multimap() { tree.emplace_back(-1); }
  int insert(std::string key, int val, int off_set = 'a') {
    int i = 0;
    tree[i].vals.push_back(val);
    for (char c : key) {
      if (tree[i].pos[c - off_set] == -1) {
        tree[i].pos[c - off_set] = (int) tree.size();
        tree.emplace_back(i);
      }
      i = tree[i].pos[c - off_set];
      tree[i].vals.push_back(val);
    }
    tree[i].key_cnt++;
    return i;
  }
  int prefix_count(int i) const {
    assert(0 <= i && i < tree.size());
    return (int) tree[i].vals.size();
  }
  const std::vector<Value> &values(int i) const {
    assert(0 <= i && i < tree.size());
    return tree[i].vals;
  }
  int parent(int i) const {
    assert(0 <= i && i < tree.size());
    return tree[i].parent;
  }
  int size() const { return (int) tree.size(); }
  int count(int i) const {
    assert(0 <= i && i < tree.size());
    return tree[i].key_cnt;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TRIE_MULTIMAP_HPP_
