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
// Aho-Corasick automaton
template<unsigned sigma_size> class AC {
  struct node {
    std::array<int, sigma_size> pos{};
    int flag = -1;
    // termed "failure function" in [Aho-Corasick 75]
    int suffix = 0;
    // keyword suffix
    int kw_suffix = -1;
  };
  std::vector<node> state;
  int kw_cnt = 0;

 public:
  AC() {
    state.emplace_back();
    state[0].suffix = -1;
  }
  int size() const { return kw_cnt; }
  template<typename T> int insert(const T *s, int len, int offset = 0) {
    int index = 0;
    for (int i = 0; i < len; ++i) {
      int j = s[i] - offset;
      if (!state[index].pos[j]) {
        state[index].pos[j] = (int) state.size();
        state.emplace_back();
      }
      index = state[index].pos[j];
    }
    if (state[index].flag == -1)
      state[index].flag = kw_cnt++;
    return state[index].flag;
  }
  int insert(const std::string &s, int offset = 'a') {
    return insert(s.c_str(), (int) s.size(), offset);
  }
  void build() {
    std::queue<int> que;
    for (int i = 0; i < sigma_size; ++i)
      if (state[0].pos[i])
        que.push(state[0].pos[i]);
    while (!que.empty()) {
      int u = que.front();
      que.pop();
      int fu = state[u].suffix;// failure function
      for (int c = 0; c < sigma_size; ++c) {
        int &v = state[u].pos[c];
        if (v) {
          que.push(v);
          int fv = state[v].suffix = state[fu].pos[c];
          state[v].kw_suffix = state[fv].flag == -1 ? state[fv].kw_suffix : fv;
        } else {
          // 把不存在的边补上
          v = state[fu].pos[c];
        }
      }
    }
  }
  // The following methods must be called after calling build().
  auto get_kw_links() const {
    std::vector<int> par(kw_cnt, -1);
    for (const node &s : state)
      if (s.flag != -1 and s.kw_suffix != -1)
        par[s.flag] = state[s.kw_suffix].flag;
    return par;
  }
  template<typename T>
  auto search_substr(const T *s, int len, int offset = 0) const {
    std::vector<int> ret;
    for (int i = 0, index = 0; i < len; ++i) {
      index = state[index].pos[s[i] - offset];
      if (state[index].flag != -1)
        ret.push_back(state[index].flag);
      else if (state[index].kw_suffix != -1)
        ret.push_back(state[state[index].kw_suffix].flag);
    }
    return ret;
  }
  auto search_substr(const std::string &s, int offset = 'a') const {
    return search_substr(s.c_str(), (int) s.size(), offset);
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_AC_AUTOMATON_HPP_
