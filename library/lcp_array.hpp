//
// Created by zjsdu on 8/9/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LCP_ARRAY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LCP_ARRAY_HPP_
#include <vector>
#include <cassert>
// Given a string s of length n and its suffix array sa, lcp_array(s, sa)
// returns an array lcp of length n-1, lcp[i] being the length of the longest
// common prefix of suffix s[i..] and suffix s[i+1..].
template<typename T>
std::vector<int> lcp_array(const T &s, const std::vector<int> &sa) {
  int n = (int) sa.size();
  assert(n >= 1);
  std::vector<int> rank(n);
  for (int i = 0; i < n; i++) {
    rank[sa[i]] = i;
  }
  std::vector<int> lcp(n - 1);
  int h = 0;
  for (int i = 0; i < n; i++) {
    if (h > 0)
      h--;
    if (rank[i] == 0)
      continue;
    int j = sa[rank[i] - 1];
    for (; j + h < n and i + h < n; h++) {
      if (s[j + h] != s[i + h])
        break;
    }
    lcp[rank[i] - 1] = h;
  }
  return lcp;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_LCP_ARRAY_HPP_
