//
// Created by zjs on 3/5/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_TRIE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_TRIE_HPP_

#include <vector>
template<int C, char alpha, int N>
struct trie {
  trie() {
    is_word.reserve(N);
    go.reserve(N);
    go.emplace_back(C, -1);
    is_word.emplace_back(false);
  }

  void insert(const char *s, int w = 1) {
    int p = 0;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - alpha;
      if (go[p][c] == -1) {
        go[p][c] = go.size();
        go.emplace_back(C, -1);
        is_word.emplace_back(false);
      }
      p = go[p][c];
    }
    is_word[p] = true;
  }

  int get(const char *s) const {
    int p = 0;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - alpha;
      if (go[p][c] == -1) {
        return -1;
      }
      p = go[p][c];
    }
    return p;
  }

  std::vector<std::vector<int>> go;
  std::vector<bool> is_word;
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_TRIE_HPP_
