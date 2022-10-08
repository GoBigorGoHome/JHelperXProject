//
// Created by zjsdu on 6/15/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SCC_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SCC_HPP_
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>
#include <type_traits.hpp>
//! @brief Find strongly connected components of a directed graph.
template<typename Graph> class SCC {
  using Arc = typename remove_all_extents_<Graph>::type;
  Graph &g;// Vertices of g are numbered from 0.
  int n;   // Number of vertices in g.
  std::function<int &(Arc &)> get_v;
  // SCCs are numbered from 0.
  std::vector<int> scc_id_, low, order, stack;
  std::vector<bool> in_stack;
  int time = 0, scc_cnt = 0;
  Graph inter_scc_arcs, scc_g;
  void dfs(int u) {
    order[u] = low[u] = ++time;
    stack.push_back(u);
    in_stack[u] = true;

    for (auto &e : g[u]) {
      int v = get_v(e);
      if (order[v] == 0) {
        dfs(v);
        low[u] = std::min(low[u], low[v]);
      } else if (in_stack[v]) {
        low[u] = std::min(low[u], order[v]);
      }
      if (not in_stack[v]) {
        inter_scc_arcs[u].push_back(e);
        get_v(inter_scc_arcs[u].back()) = scc_id_[v];
      }
    }

    if (order[u] == low[u]) {
      scc_g.emplace_back();
      while (true) {
        int v = stack.back();
        stack.pop_back();
        in_stack[v] = false;
        scc_id_[v] = scc_cnt;
        scc_g.back().insert(scc_g.back().end(), inter_scc_arcs[v].begin(),
                            inter_scc_arcs[v].end());
        if (u == v)
          break;
      }
      ++scc_cnt;
    }
  }

 public:
  SCC(
      Graph &g,
      std::function<int &(Arc &e)> get_v = [](Arc &e) -> int & { return e; })
      : g(g), n((int) g.size()), get_v(get_v), scc_id_(n, -1), low(n), order(n),
        in_stack(n), inter_scc_arcs(n) {}

  template<typename Fun = bool (*)(int)>
  Graph find_scc(Fun ignore_ = [](int) { return false; }) {
    for (int i = 0; i < n; i++) {
      if (not ignore_(i) and scc_id_[i] == -1) {
        dfs(i);
      }
    }
    return scc_g;
  }

  Graph find_scc_from(int start) {
    assert(0 <= start and start < n);
    assert(scc_id_[start] == -1);
    dfs(start);
    return scc_g;
  }
  /// scc_id 越大，拓扑序越小。
  int scc_id(int u) const {
    assert(0 <= u and u < n);
    return scc_id_[u];
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SCC_HPP_
