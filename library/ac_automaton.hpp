//
// Created by zjsdu on 11/3/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_HPP_
#include <vector>
#include <array>
#include <queue>
#include <algorithm>
#include <string>
#include <cassert>
// Aho-Corasick automaton
template<unsigned sigma_size> class AC {
  std::vector<std::array<int, sigma_size>> pos;
  std::vector<int> flag, kw_suffix;
  int kw_cnt = 0;

  void make_new_node() {
    pos.push_back({});
    flag.push_back(-1);
    kw_suffix.push_back(-1);
  }

 public:
  AC() { make_new_node(); }
  int size() const { return kw_cnt; }
  template<typename T> int insert(const T *s, int len, int offset = 0) {
    int i = 0;
    for (int _ = 0; _ < len; ++_) {
      int j = s[_] - offset;
      if (!pos[i][j]) {
        pos[i][j] = (int) pos.size();
        make_new_node();
      }
      i = pos[i][j];
    }
    if (flag[i] == -1)
      flag[i] = kw_cnt++;
    return flag[i];
  }

  int insert(const std::string &s, int offset = 'a') {
    return insert(s.c_str(), (int) s.size(), offset);
  }

  void build() {
    std::queue<std::pair<int, int>> que;// (node_id, suffix_link)
    for (int i = 0; i < sigma_size; ++i)
      if (pos[0][i])
        que.emplace(pos[0][i], 0);
    while (!que.empty()) {
      auto [u, suffix_u] = que.front();
      que.pop();
      for (int c = 0; c < sigma_size; ++c) {
        int &v = pos[u][c];
        if (v) {
          int suffix_v = pos[suffix_u][c];
          que.emplace(v, suffix_v);
          if (flag[suffix_v] != -1)
            kw_suffix[v] = suffix_v;
          else
            kw_suffix[v] = kw_suffix[suffix_v];
        } else {
          // 把不存在的边补上
          v = pos[suffix_u][c];
        }
      }
    }
  }

  int go(int i, int j) const {
    assert(0 <= i && i < pos.size());
    assert(0 <= j && j < sigma_size);
    return pos[i][j];
  }

  bool is_keyword(int i) const {
    assert(0 <= i && i < pos.size());
    return flag[i] != -1 || kw_suffix[i] != -1;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_HPP_
