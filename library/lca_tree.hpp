//
// Created by zjsdu on 2/20/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_TREE_HPP_
#include <dfs_tree.hpp>
#include <algorithm>
class lca_tree : public dfs_tree {
  std::vector<std::vector<int>> a;
  int h;

 public:
  lca_tree(int n) : dfs_tree(n) {}
  void build_lca(int root) {
    dfs(root);
    int max_depth = *max_element(depth_.begin(), depth_.end());
    for (h = 0; max_depth > 0; ++h)
      max_depth /= 2;
    a.assign(n, std::vector<int>(h));
    for (int i = 0; i < n; ++i)
      a[i][0] = parent_[i];
    for (int j = 1; j < h; ++j)
      for (int i = 0; i < n; ++i)
        a[i][j] = a[i][j - 1] == -1 ? -1 : a[a[i][j - 1]][j - 1];
  }

  int ancestor(int u, int d) const {
    assert(0 <= u and u < n);
    assert(0 <= d);
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
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_TREE_HPP_
