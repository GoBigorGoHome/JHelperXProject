//
// Created by zjs on 2021/12/26.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_FOREST_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_FOREST_HPP_
#include <graph.hpp>
#include <algorithm>

class dfs_forest : public graph {
  std::vector<int> order;
  int counter;
  std::vector<int> L, R;
  std::vector<int> sz;

 protected:
  std::vector<int> depth_;
  std::vector<int> pv;// parent (vertex)
  std::vector<int> pe;// parent edge
  std::vector<int> root_;

 private:
  void do_dfs(int u) {
    order.push_back(u);
    L[u] = ++counter;
    sz[u] = 1;
    for (int i : g[u]) {
      int v = edges[i].first ^ edges[i].second ^ u;
      if (depth_[v] == -1) {
        depth_[v] = depth_[u] + 1;
        pv[v] = u;
        pe[v] = i;
        root_[v] = root_[u];
        do_dfs(v);
        sz[u] += sz[v];
      }
    }
    R[u] = counter;
  }

 public:
  explicit dfs_forest(int n)
      : graph(n), L(n, -1), R(n, -1), sz(n), depth_(n, -1), pv(n, -1),
        pe(n, -1), root_(n, -1) {}

  void dfs_all() {
    assert(order.empty());
    counter = -1;
    for (int i = 0; i < n; i++)
      if (depth_[i] == -1) {
        depth_[i] = 0;
        root_[i] = i;
        do_dfs(i);
      }
    assert(order.size() == n);
  }

  /// @brief Is u an ancestor of v?
  bool is_ancestor(int u, int v) const { return L[u] <= L[v] and R[v] <= R[u]; }

  /// @brief Is u a descendant of v?
  bool is_descendant(int u, int v) const { return is_ancestor(v, u); }

  int depth(int v) const {
    assert(not order.empty());
    assert(0 <= v and v < n);
    return depth_[v];
  }

  int root(int u) const {
    assert(0 <= u and u < n);
    assert(root_[u] != -1);
    return root_[u];
  }

  /// @brief returns a list of all vertices in reversed order of time when a
  /// vertex is discovered by the DFS. The latest discovered vertex comes first.
  std::vector<int> bottom_up() const {
    assert(not order.empty());
    auto res = order;
    std::reverse(res.begin(), res.end());
    return res;
  }

  int size(int v) const { return sz[v]; }

  int parent(int v) const { return pv[v]; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_FOREST_HPP_
