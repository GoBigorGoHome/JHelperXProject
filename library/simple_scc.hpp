//
// Created by zjsdu on 2021/12/10.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SIMPLE_SCC_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SIMPLE_SCC_HPP_
#include <vector>
#include <algorithm>
#include <cassert>

//! @brief Find strongly connected components of a directed graph.
template<typename Graph> class SCC {
  const Graph& g;// Vertices of g are numbered from 0.
  int n;         // Number of vertices in g.
  // SCCs are numbered from 0.
  std::vector<int> scc_id_, low, order, stack;
  std::vector<bool> in_stack;
  int time = 0, scc_cnt = 0;

  void dfs(int u) {
    order[u] = low[u] = ++time;
    stack.push_back(u);
    in_stack[u] = true;

    for (int v : g[u]) {
      if (order[v] == 0) {
        dfs(v);
        low[u] = std::min(low[u], low[v]);
      } else if (in_stack[v]) {
        low[u] = std::min(low[u], order[v]);
      }
    }

    if (order[u] == low[u]) {
      while (true) {
        int v = stack.back();
        stack.pop_back();
        in_stack[v] = false;
        scc_id_[v] = scc_cnt;
        if (u == v)
          break;
      }
      ++scc_cnt;
    }
  }

 public:
  explicit SCC(const Graph& g)
      : g(g), n((int) g.size()), scc_id_(n, -1), low(n), order(n), in_stack(n) {
  }

  void find_scc() {
    for (int i = 0; i < n; i++) {
      if (scc_id_[i] == -1) {
        dfs(i);
      }
    }
  }

  void find_scc_from(int start) {
    assert(0 <= start and start < n);
    assert(scc_id_[start] == -1);
    dfs(start);
  }

  int scc_id(int u) const {
    assert(0 <= u and u < n);
    return scc_id_[u];
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SIMPLE_SCC_HPP_
