//
// Created by zjs on 2023/2/9.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DSU_WITH_ROLLBACK_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DSU_WITH_ROLLBACK_HPP_

#include <vector>
#include <cassert>

class dsu_with_rollback {
  std::vector<int> ps;                    // parent or size
  std::vector<std::pair<int, int>> record;// merge record
  int ecnt = 0;                           // number of edges added

 public:
  explicit dsu_with_rollback(int n) : ps(n, -1), record(n - 1) {}

  void clear() {
    fill(ps.begin(), ps.end(), -1);
    ecnt = 0;
  }

  int root(int u) const {
    while (ps[u] >= 0)
      u = ps[u];
    return u;
  }

  int size(int u) const { return -ps[root(u)]; }

  int edge_cnt() const { return ecnt; }

  bool merge(int u, int v) {
    u = root(u);
    v = root(v);
    if (u == v)
      return 0;
    if (ps[u] < ps[v])
      std::swap(u, v);
    ps[v] += ps[u];
    record[ecnt++] = {u, ps[u]};
    ps[u] = v;
    return 1;
  }

  void rollback() {
    assert(ecnt > 0);
    auto [u, sz] = record[--ecnt];
    int v = ps[u];
    ps[u] = sz;
    ps[v] -= sz;
  }

  void rollback_to(int edge_cnt) {
    while (ecnt > edge_cnt)
      rollback();
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DSU_WITH_ROLLBACK_HPP_
