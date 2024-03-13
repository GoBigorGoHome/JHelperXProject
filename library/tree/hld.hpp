//
// Created by zjs on 3/13/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_HLD_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_HLD_HPP_

// 例题：https://cses.fi/problemset/task/2134
// how to use: hld_init(n); add_edges(u, v); hld_build();
#include <vector>
#include <cassert>

bool ready_ = false;
bool init_ = false;
int id_ = 0;
int n_;
std::vector<std::vector<int>> g;
std::vector<int> pv;
std::vector<int> dep;
std::vector<int> top;
std::vector<int> sz;
std::vector<int> id;
std::vector<int> node_;

void hld_init(int n) {
  n_ = n;
  g.assign(n, std::vector<int>());
  pv.assign(n, -1);
  dep.assign(n, 0);
  top.assign(n, 0);
  sz.assign(n, 0);
  id.assign(n, 0);
  node_.assign(n, 0);
  init_ = true;
}

void sz_dfs(int u) {
  sz[u] = 1;
  for (int v : g[u]) {
    if (!sz[v]) {
      pv[v] = u;
      dep[v] = dep[u] + 1;
      sz_dfs(v);
      sz[u] += sz[v];
    }
  }
}

void hld_dfs(int u, int _top, int p = -1) {
  node_[id_] = u;
  id[u] = id_++;
  top[u] = _top;
  int hc = -1;// heavy child
  for (int v : g[u]) {
    if (v != p && (hc == -1 || sz[v] > sz[hc]))
      hc = v;
  }
  if (hc == -1)
    return;
  hld_dfs(hc, _top, u);
  for (int v : g[u])
    if (v != p && v != hc)
      hld_dfs(v, v, u);
}

void add_edge(int u, int v) {
  assert(init_);
  g[u].push_back(v);
  g[v].push_back(u);
};

void hld_build() {
  assert(init_);
  if (ready_)
    return;
  sz_dfs(0);
  hld_dfs(0, 0);
  ready_ = true;
}

int lca(int a, int b) {
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]])
      a = pv[top[a]];
    else
      b = pv[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}

int ancestor(int a, int k) {
  if (k > dep[a])
    return -1;
  while (k > dep[a] - dep[top[a]]) {
    k -= dep[a] - dep[top[a]] + 1;
    a = pv[top[a]];
  }
  return node_[id[a] - k];
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_HLD_HPP_
