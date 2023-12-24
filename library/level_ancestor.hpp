//
// Created by zjs on 12/19/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LEVEL_ANCESTOR_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LEVEL_ANCESTOR_HPP_
#include <vector>
// also known as doubling technique, jump pointers
class level_ancestor {
  int n;
  int log;
  std::vector<std::vector<int>> a;
 public:
  explicit level_ancestor(const std::vector<int> &parent)
      : n((int) parent.size()) {
    log = 0;
    int t = 1;
    while (t < n) {
      log++;
      t *= 2;
    }
    a.assign(log, std::vector<int>(n, -1));
    if (log) {
      a[0] = parent;
      for (int i = 1; i < log; i++)
        for (int j = 0; j < n; j++)
          if (a[i - 1][j] != -1)
            a[i][j] = a[i - 1][a[i - 1][j]];
    }
  }

  int ancestor(int u, int k) const {
    if (k >= n)
      return -1;
    int v = u;
    for (int i = 0; i < log; i++)
      if (k >> i & 1) {
        v = a[i][v];
        if (v == -1)
          break;
      }
    return v;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_LEVEL_ANCESTOR_HPP_
