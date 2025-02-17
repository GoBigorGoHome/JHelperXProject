//
// Created by zjs on 2/22/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_TREE_HPP_

#include <vector>
#include <cassert>

struct lca_tree {
  bool ready = false;
  int cnt = 0;
  std::vector<std::vector<int>> adj;
  std::vector<int> pre;
  std::vector<int> dep, num, sz, pv, top;

  lca_tree(int n)
      : adj(n), pre(n), dep(n), num(n, -1), sz(n), pv(n, -1), top(n) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void dfs(int u) {
    num[u] = cnt;
    pre[cnt] = u;
    cnt++;
    sz[u] = 1;
    for (int v : adj[u])
      if (num[v] == -1) {
        // 易错点：计算dep[v]要在调用dfs(v)之前！
        dep[v] = dep[u] + 1;
        pv[v] = u;
        dfs(v);
        sz[u] += sz[v];
      }
  }

  void build(int root) {
    dfs(root);
    for (int u : pre) {
      if (pv[u] == -1 || sz[u] * 2 < sz[pv[u]])
        top[u] = u;
      else
        top[u] = top[pv[u]];
    }
    ready = true;
  }

  int lca(int u, int v) {
    assert(ready);
    while (top[u] != top[v]) {
      if (dep[top[u]] > dep[top[v]])
        u = pv[top[u]];
      else
        v = pv[top[v]];
    }
    return dep[u] < dep[v] ? u : v;
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_TREE_HPP_
