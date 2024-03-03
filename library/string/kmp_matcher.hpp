//
// Created by zjs on 3/3/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_KMP_MATCHER_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_KMP_MATCHER_HPP_
// 例题：
// https://atcoder.jp/contests/abc343/tasks/abc343_g
// https://atcoder.jp/contests/abc257/tasks/abc257_g
#include <string>
#include <vector>
struct KMP_matcher {
  std::string P;
  int m;
  std::vector<int> fail;
  KMP_matcher(std::string P) : P(P), m(P.size()), fail(m + 1) {
    fail[0] = -1;
    int t = 0;
    for (int j = 1; j < m; j++) {
      // 此时t 是 P[0..j-1] 的最长前后缀的长度
      if (P[j] != P[t])
        fail[j] = t;
      else
        fail[j] = fail[t];
      while (t != -1 and P[t] != P[j])
        t = fail[t];
      ++t;
      // 此时t是P[0..j]的最长前后缀的长度
    }
    // 此时 t 是 P 的最长前后缀的长度。
    fail[m] = t;
  }
  // P是T的子串吗？
  bool test_match(std::string T) {
    int j = 0;
    for (char c : T) {
      while (j != -1 and P[j] != c)
        j = fail[j];
      j++;
      if (j == m)
        return true;
    }
    return false;
  }

  std::vector<int> match(std::string T) {
    std::vector<int> res(T.size());
    // res[i]：P和T[0..i]的最长前后缀的长度。
    int j = 0;
    for (int i = 0; i < (int) T.size(); i++) {
      while (j != -1 and P[j] != T[i])
        j = fail[j];
      j++;
      res[i] = j;
    }
    return res;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_KMP_MATCHER_HPP_
