//
// Created by zjs on 6/27/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_OFFLINE_QUERY_PARALLEL_BINARY_SEARCH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_OFFLINE_QUERY_PARALLEL_BINARY_SEARCH_HPP_
#include <vector>
#include <algorithm>
// 无修改操作的整体二分算法。
template<typename F1, typename F2, typename F3>
std::vector<int> parallel_binary_search(int Q, int ok, int ng, F1 init, F2 upd,
                                        F3 check) {
  int T = std::max(ok, ng);
  std::vector<int> OK(Q, ok), NG(Q, ng);
  while (1) {
    std::vector<int> check_t(Q, -1);
    for (int q = 0; q < Q; q++) {
      if (abs(OK[q] - NG[q]) > 1) {
        check_t[q] = (OK[q] + NG[q]) / 2;
      }
    }
    std::vector<int> cnt(T + 1);
    for (int t : check_t) {
      if (t != -1)
        cnt[t + 1]++;
    }
    for (int t = 0; t < T; t++)
      cnt[t + 1] += cnt[t];
    int total = cnt.back();
    if (total == 0)
      break;
    // 此时counter[t]是中点小于t的询问的数量
    std::vector<int> csr(total);
    // 把Q个询问按中点从小到大排序
    for (int q = 0; q < Q; q++) {
      int &t = check_t[q];
      if (t != -1) {
        csr[cnt[t]++] = q;
      }
    }

    init();
    int t = 0;
    for (int q : csr) {
      while (t < check_t[q]) {
        upd(t++);
      }
      if (check(q))
        OK[q] = t;
      else
        NG[q] = t;
    }
  }
  return OK;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_OFFLINE_QUERY_PARALLEL_BINARY_SEARCH_HPP_
