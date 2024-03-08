//
// Created by zjs on 3/5/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_AC_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_AC_HPP_

#include <vector>
#include <queue>
#include <cassert>
#include <string>

template<int C, char alpha, int N>
struct AhoCorasick {
  AhoCorasick() {
    word_cnt = 0;
    GO.reserve(N);
    word_id.reserve(N);
    GO.emplace_back(C, -1);
    word_id.emplace_back(-1);
  }

  int insert(const char *s) {
    int p = 0;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - alpha;
      if (GO[p][c] == -1) {
        GO[p][c] = GO.size();
        GO.emplace_back(C, -1);
        word_id.emplace_back(-1);
      }
      p = GO[p][c];
    }

    int &id = word_id[p];
    if (id == -1)
      id = word_cnt;
    word_cnt++;
    return id;
  }

  int insert(std::string s) { return insert(s.c_str()); }

  int get(const char *s) const {
    int p = 0;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - alpha;
      if (GO[p][c] == -1) {
        return -1;
      }
      p = GO[p][c];
    }
    return p;
  }

  void initialize() {
    last_word.resize(word_cnt);
    // fail[0] is not defined.
    std::vector<int> fail(GO.size());
    std::queue<int> q;
    for (int c = 0; c < C; ++c) {
      int &v = GO[0][c];
      if (~v) {
        q.push(v);
      }
      (~v ? fail[v] : v) = 0;
    }

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      int &id = word_id[u];
      (~id ? last_word[id] : id) = word_id[fail[u]];
      for (int c = 0; c < C; ++c) {
        int &v = GO[u][c];
        if (~v) {
          q.push(v);
        }
        (~v ? fail[v] : v) = GO[fail[u]][c];
      }
    }
  }

  int go(int p, char c) { return GO[p][c - alpha]; }

  std::vector<int> toposort() {
    std::vector<int> deg(word_cnt, 0);
    for (int i : last_word)
      if (~i)
        deg[i]++;
    std::vector<int> x;
    for (int i = 0; i < word_cnt; i++) {
      if (deg[i] == 0) {
        x.push_back(i);
      }
    }
    for (int ptr = 0; ptr < (int) x.size(); ptr++) {
      int i = x[ptr];
      int j = last_word[i];
      if (~j && --deg[j] == 0) {
        x.push_back(j);
      }
    }
    return x;
  }

  std::vector<std::vector<int>> GO;
  std::vector<int> word_id;
  std::vector<int> last_word;
  int word_cnt;
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_AC_HPP_
