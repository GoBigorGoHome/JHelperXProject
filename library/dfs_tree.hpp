//
// Created by zjsdu on 2/20/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_TREE_HPP_
#include <tree.hpp>
class dfs_tree : public tree {

  std::vector<int> order;

 protected:
  std::vector<int> depth_;
  std::vector<int> parent;

 private:
  void do_dfs(int u) {
    order.push_back(u);
    for (int v : g[u]) {
      if (depth_[v] == -1) {
        depth_[v] = depth_[u] + 1;
        parent[v] = u;
        do_dfs(v);
      }
    }
  }

 public:
  dfs_tree(int n) : tree(n), parent(n), depth_(n) {}
  void dfs(int u) {
    assert(0 <= u and u < n);
    order.clear();
    for (int i = 0; i < n; i++)
      depth_[i] = -1;
    parent[u] = -1;
    depth_[u] = 0;
    do_dfs(u);
    assert(order.size() == n);
  }

  template<typename F>
  //! @param f void(int parent, int child)
  void accumulate_bottom_up(F f) const {
    assert(not order.empty());
    for (int i = n - 1; i > 0; --i)
      f(parent[order[i]], order[i]);
  }

  int depth(int v) const {
    assert(not order.empty());
    assert(0 <= v and v < n);
    return depth_[v];
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_TREE_HPP_
