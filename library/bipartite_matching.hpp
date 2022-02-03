//
// Created by zjsdu on 2022/2/3.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BIPARTITE_MATCHING_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BIPARTITE_MATCHING_HPP_

#include <vector>
#include <cassert>

class bipartite_matching {
  using Edge = std::pair<int, int>;
  using Edges = std::vector<Edge>;
  using Graph = std::vector<std::vector<int>>;
  int n, m;
  std::vector<int> used, p, q;
  Graph g;

 public:
  bipartite_matching(int n, int m) : n(n), m(m), used(n), p(n, -1), q(m, -1), g(n) {}

  void add_edge(int u, int v) {
    assert(0 <= u and u < n);
    assert(0 <= v and v < m);
    g[u].push_back(v);
  }

  /// @brief Find a matching of maximum cardinality.
  /// @returns the maximum cardinality.
  int build() {
    int ret = 0;
    bool ok = true;
    int j = 1;

    while (ok) {
      ok = false;
      // try to find a set of disjoint augmenting paths.
      for (int i = 0; i < n; ++i)
        if (p[i] == -1 && dfs(i, j))
          ok = true, ++ret;
      ++j;
    }
    return ret;
  }

  /// @returns a matching of maximum cardinality.
  Edges get_matching() const {
    Edges ret;
    for (int i = 0; i < n; ++i)
      if (p[i] != -1)
        ret.emplace_back(i, p[i]);
    return ret;
  }

 private:
  bool dfs(int v, int k) {
    if (used[v] == k)
      return false;
    used[v] = k;
    for (int u : g[v]) {
      if (q[u] == -1 || dfs(q[u], k)) {
        q[u] = v;
        p[v] = u;
        return true;
      }
    }
    return false;
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BIPARTITE_MATCHING_HPP_
