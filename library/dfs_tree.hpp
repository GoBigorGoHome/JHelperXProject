//
// Created by zjsdu on 2/20/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_TREE_HPP_
#include <tree.hpp>
class dfs_tree : public tree {
  std::vector<int> order;
  int counter;
  std::vector<int> L, R;
  std::vector<int> sz;

 protected:
  std::vector<int> depth_;
  std::vector<int> parent_;

 private:
  void do_dfs(int u) {
    order.push_back(u);
    L[u] = ++counter;
    sz[u] = 1;
    for (int v : g[u]) {
      if (depth_[v] == -1) {
        depth_[v] = depth_[u] + 1;
        parent_[v] = u;
        do_dfs(v);
        sz[u] += sz[v];
      }
    }
    R[u] = counter;
  }

 public:
  dfs_tree(int n) : tree(n), parent_(n), depth_(n), L(n), R(n), sz(n) {}

  void dfs(int u) {
    assert(0 <= u and u < n);
    order.clear();
    for (int i = 0; i < n; i++)
      depth_[i] = -1;
    parent_[u] = -1;
    depth_[u] = 0;
    counter = -1;
    do_dfs(u);
    assert(order.size() == n);
  }

  /// @brief Is u an ancestor of v?
  bool is_ancestor(int u, int v) const { return L[u] <= L[v] and R[v] <= R[u]; }

  /// @brief Is u a descendant of v?
  bool is_descendant(int u, int v) const { return is_ancestor(v, u); }

  template<typename F>
  //! @param f void(int parent, int child)
  void accumulate_bottom_up(F f) const {
    assert(not order.empty());
    for (int i = n - 1; i > 0; --i)
      f(parent_[order[i]], order[i]);
  }

  int depth(int v) const {
    assert(not order.empty());
    assert(0 <= v and v < n);
    return depth_[v];
  }

  int size(int v) const { return sz[v]; }

  int parent(int v) const { return parent_[v]; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_TREE_HPP_
