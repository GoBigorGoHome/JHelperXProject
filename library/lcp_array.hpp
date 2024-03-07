//
// Created by zjsdu on 8/9/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LCP_ARRAY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LCP_ARRAY_HPP_
#include <vector>
#include <cassert>
// Given a string s of length n and its suffix array sa, lcp_array(s, sa)
// returns an array lcp of length n, lcp[i] being the length of the longest
// common prefix of suffix s[i..] and suffix s[i-1..], and lcp[0] is 0.
template<typename T>
std::vector<int> lcp_array(const T &s, const std::vector<int> &sa) {
  int n = (int) sa.size();
  std::vector<int> pos(n);// pos[i]：后缀i在sa里哪个位置。
  for (int i = 0; i < n; i++) {
    pos[sa[i]] = i;
  }
  std::vector<int> lcp(n);
  int h = 0;
  for (int i = 0; i < n; i++) {
    if (pos[i] == 0)
      continue;
    int j = sa[pos[i] - 1];
    h = std::max(0, h - 1);
    while (j + h < n && i + h < n && s[j + h] == s[i + h]) {
      h++;
    }
    lcp[pos[i]] = h;
  }
  return lcp;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_LCP_ARRAY_HPP_
