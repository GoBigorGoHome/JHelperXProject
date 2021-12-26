//
// Created by zjs on 2021/12/26.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_FOREST_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_FOREST_HPP_

#include <dfs_forest.hpp>
class lca_forest : public dfs_forest {

  std::vector<std::vector<int>> a;
  int h;

 public:
  lca_forest(int n) : dfs_forest(n) {}
  void build_lca() {
    dfs_all();
    int max_depth = *max_element(depth_.begin(), depth_.end());
    for (h = 0; max_depth > 0; ++h)
      max_depth /= 2;
    a.assign(n, std::vector<int>(h));
    for (int i = 0; i < n; ++i)
      a[i][0] = pv[i];
    for (int j = 1; j < h; ++j)
      for (int i = 0; i < n; ++i)
        a[i][j] = a[i][j - 1] == -1 ? -1 : a[a[i][j - 1]][j - 1];
  }

  int ancestor(int u, int d) const {
    assert(0 <= u and u < n);
    assert(d >= 0);
    assert(not a.empty());
    if (d > depth_[u])
      return -1;
    for (int i = 0; i < h; ++i) {
      if (d >> i & 1) {
        u = a[u][i];
      }
    }
    return u;
  }

  int lca(int u, int v) const {
    assert(not a.empty());
    if (depth_[u] < depth_[v])
      std::swap(u, v);
    u = ancestor(u, depth_[u] - depth_[v]);
    if (u == v)
      return u;
    for (int i = h - 1; i >= 0; i--)
      if (a[u][i] != a[v][i])
        u = a[u][i], v = a[v][i];
    return a[u][0];
  }

  std::vector<int> path(int from, int to) {
    assert(not a.empty());
    assert(root(from) == root(to));
    int anc = lca(from, to);
    std::vector<int> p1, p2;
    while (from != anc) {
      p1.push_back(pe[from]);
      from = pv[from];
    }
    while (to != anc) {
      p2.push_back(pe[to]);
      to = pv[to];
    }
    p1.insert(p1.end(), p2.rbegin(), p2.rend());
    return p1;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_FOREST_HPP_
