//
// Created by zjs on 9/12/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_2_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_2_HPP_
#include <array>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <cassert>

// Aho-Corasick automaton
template<unsigned sigma_size> class AC {
  struct Node {
    std::vector<int> children;
    std::array<int, sigma_size> pos{};
    int kw_id = -1;
    int suffix = 0;
    int kw_suffix = -1;// keyword suffix
    int depth = 0;
  };
  std::vector<Node> trie;
  int kw_cnt = 0;
  int done_build = 0;

 public:
  AC() : trie(1) {}

  int size() const { return kw_cnt; }

  int trie_size() const { return (int) trie.size(); }

  template<typename T> int insert(const T *s, int len, int offset = 0) {
    int i = 0;
    for (int _ = 0; _ < len; ++_) {
      int j = s[_] - offset;
      if (!trie[i].pos[j]) {
        trie[i].pos[j] = (int) trie.size();
        trie[i].children.push_back((int) trie.size());
        trie.emplace_back();
        trie.back().depth = _ + 1;
      }
      i = trie[i].pos[j];
    }
    if (trie[i].kw_id == -1)
      trie[i].kw_id = kw_cnt++;
    return trie[i].kw_id;
  }

  int insert(const std::string &s, int offset = 'a') {
    return insert(s.c_str(), (int) s.size(), offset);
  }

  const Node &node(int i) const { return trie[i]; }

  void build() {
    trie[0].suffix = -1;
    std::queue<int> q;
    for (int i = 0; i < sigma_size; i++)
      if (trie[0].pos[i] != 0)
        q.push(trie[0].pos[i]);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i = 0; i < sigma_size; i++) {
        int &v = trie[u].pos[i];
        int j = trie[trie[u].suffix].pos[i];
        if (v == 0)
          v = j;
        else {
          q.push(v);
          trie[v].suffix = j;
          trie[v].kw_suffix = trie[j].kw_id == -1 ? trie[j].kw_suffix : j;
        }
      }
    }
    done_build = 1;
  }

  int go(int i, int j) const {
    assert(0 <= i && i < trie.size());
    assert(0 <= j && j < sigma_size);
    return trie[i].pos[j];
  }

  bool is_keyword(int i) const {
    assert(0 <= i && i < trie.size());
    return trie[i].kw_id != -1 || trie[i].kw_suffix != -1;
  }

  int depth(int i) const {
    assert(done_build);
    assert(0 <= i && i < trie.size());
    return trie[i].depth;
  }

  int suffix(int i) const {
    assert(0 <= i && i < trie.size());
    return trie[i].suffix;
  }

  const std::vector<int> &children(int i) const {
    assert(0 <= i && i < trie.size());
    return trie[i].children;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_2_HPP_
