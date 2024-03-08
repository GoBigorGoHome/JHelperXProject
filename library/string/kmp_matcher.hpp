//
// Created by zjs on 3/3/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_KMP_MATCHER_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_KMP_MATCHER_HPP_
// 例题：
// https://cses.fi/problemset/task/1753
// https://cses.fi/problemset/task/1732
// https://atcoder.jp/contests/abc343/tasks/abc343_g
// https://atcoder.jp/contests/abc257/tasks/abc257_g

#include <string>
#include <vector>

struct KMP_matcher {
  std::string P;
  int m;
  // fail[j]：当T[i]和P[j]失配时，下一步要拿T[i]和P[fail[j]]比较
  std::vector<int> fail;
  std::vector<int> border;

  KMP_matcher(std::string P) : P(P), m((int) P.size()), fail(m + 1), border(m) {
    fail[0] = -1;
    border[0] = 0;
    int k = 0;
    for (int j = 1; j < m; j++) {
      // 此时 k 是 P[0..j-1] 的最长前后缀的长度
      if (P[j] != P[k])
        fail[j] = k;
      else
        fail[j] = fail[k];
      while (k != -1 and P[k] != P[j])
        k = fail[k];
      border[j] = ++k;
      // 此时 k 是P[0..j]的最长前后缀的长度
    }
    // 此时 k 是 P 的最长前后缀的长度。
    fail[m] = k;
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
      while (j != -1 and P[j] != T[i])// P[m]='\0'
        j = fail[j];
      j++;
      res[i] = j;
    }
    return res;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_KMP_MATCHER_HPP_
