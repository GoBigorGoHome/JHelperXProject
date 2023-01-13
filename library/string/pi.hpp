//
// Created by zjs on 1/13/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_PI_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_PI_HPP_
#include <vector>
#include <string>
#include <cassert>
// 对于i=0,1,...,|s|-1，pi[i]是s[0..i]的最长前后缀的长度。
std::vector<int> get_pi(const std::string &s) {
  assert(!s.empty());
  int n = (int) s.size();
  std::vector<int> pi(n);
  pi[0] = 0;
  int j = 0;
  for (int i = 1; i < n; i++) {
    while (s[i] != s[j]) {
      if (j == 0) {
        j = -1;
        break;
      }
      j = pi[j - 1];
    }
    pi[i] = ++j;
  }
  return pi;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_PI_HPP_
