//
// Created by zjsdu on 8/9/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SUFFIX_ARRAY_2_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SUFFIX_ARRAY_2_HPP_
#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
// A simple implementation of the prefix doubling algorithm for suffix array
// construction.
template<typename T>
std::vector<int> suffix_array(const T &s, int n) {
  assert(n > 0);
  std::vector<int> suf(n);
  std::iota(suf.begin(), suf.end(), 0);
  // TODO: figure out why the function is much slower if s is captured by copy.
  std::sort(suf.begin(), suf.end(), [&s](int x, int y) { return s[x] < s[y]; });
  std::vector<int> rank_(n), rank(n);
  std::vector<int> suf_(n);
  rank[suf[0]] = 0;
  for (int i = 1; i < n; ++i) {
    rank[suf[i]] = rank[suf[i - 1]] + (s[suf[i - 1]] != s[suf[i]]);
  }
  std::vector<int> cnt(n);
  for (int len = 1; rank[suf.back()] + 1 < n; len *= 2) {
    int m = rank[suf.back()] + 1;
    // Since we have ranks of all substrings of length len, we can view a
    // substring of length 2*len as a 2-digit number and use radix sort to sort
    // all substrings of length 2*len on the 2-digit number.
    for (int i = 0; i < m; i++)
      cnt[i] = 0;
    for (int v : rank)
      cnt[v]++;
    for (int i = 1; i < m; i++)
      cnt[i] += cnt[i - 1];
    for (auto it = suf.rbegin(); it != suf.rend(); ++it) {
      if (*it >= len) {
        suf_[--cnt[rank[*it - len]]] = *it - len;
      }
    }
    for (int i = n - len; i < n; i++) {
      suf_[--cnt[rank[i]]] = i;
    }
    rank_[suf_[0]] = 0;
    for (int i = 1; i < n; i++) {
      int x = suf_[i - 1], y = suf_[i];
      rank_[y] = rank_[x]
          + (x + len >= n or y + len >= n or rank[x] != rank[y]
             or rank[x + len] != rank[y + len]);
    }
    std::swap(rank, rank_);
    std::swap(suf, suf_);
  }
  return suf;
}

template<typename T>
std::vector<int> suffix_array(const T &s) {
  return suffix_array(s, (int) std::size(s));
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SUFFIX_ARRAY_2_HPP_
