//
// Created by zjs on 2021/12/26.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_FOREST_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LCA_FOREST_HPP_

#include <dfs_forest.hpp>
class lca_forest : public dfs_forest {
  std::vector<int> head;
  bool ready = false;

 public:
  lca_forest(int n) : dfs_forest(n), head(n) {}
  void build() {
    if (ready)
      return;
    dfs_all();
    for (int u : pre_order) {
      if (pv[u] == -1 || sz[u] * 2 < sz[pv[u]])
        head[u] = u;
      else
        head[u] = head[pv[u]];
    }
    ready = true;
  }

  int lca(int u, int v) const {
    assert(ready);
    if (root(u) != root(v))
      return -1;
    while (head[u] != head[v]) {
      if (depth_[head[u]] > depth_[head[v]])
        u = pv[head[u]];
      else
        v = pv[head[v]];
    }
    return depth_[u] < depth_[v] ? u : v;
  }

  int dist(int u, int v) {
    assert(ready);
    return depth_[u] + depth_[v] - 2 * depth_[lca(u, v)];
  }

  int rooted_lca(int a, int b, int c) {
    assert(ready);
    return lca(a, b) ^ lca(b, c) ^ lca(c, a);
  }

  // sequence of edges on the from-to path
  std::vector<int> path(int from, int to) {
    assert(ready);
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
