//
// Created by zjs on 2021/12/26.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_FOREST_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_FOREST_HPP_
#include <graph.hpp>
#include <algorithm>
#include <unordered_map>

class dfs_forest : public graph {

  std::vector<int> in, out, low;
  std::unordered_map<int, int> bridge;
  bool ready = false;

 protected:
  std::vector<int> pre_order;
  std::vector<int> sz;
  std::vector<int> depth_;
  std::vector<int> pv;// parent (vertex)
  std::vector<int> pe;// parent edge
  std::vector<int> root_;

 private:
  void do_dfs(int u, int p, int &num) {
    pre_order[num] = u;
    low[u] = in[u] = num++;
    sz[u] = 1;
    for (auto [v, edge_id] : g[u]) {
      if (edge_id == p)
        continue;
      if (depth_[v] == -1) {
        depth_[v] = depth_[u] + 1;
        pv[v] = u;
        pe[v] = edge_id;
        root_[v] = root_[u];
        do_dfs(v, edge_id, num);
        if (low[v] == in[v])
          bridge[edge_id] = v;
        low[u] = std::min(low[u], low[v]);
        sz[u] += sz[v];
      } else {
        low[u] = std::min(low[u], in[v]);
      }
    }
    out[u] = num;
  }

 public:
  explicit dfs_forest(int n)
      : graph(n), in(n, -1), out(n, -1), low(n, -1), pre_order(n), sz(n),
        depth_(n, -1), pv(n, -1), pe(n, -1), root_(n, -1) {}

  void dfs_all() {
    if (ready)
      return;
    int num = 0;
    for (int i = 0; i < n; i++)
      if (depth_[i] == -1) {
        depth_[i] = 0;
        root_[i] = i;
        do_dfs(i, -1, num);
      }
    assert(num == n);
    ready = true;
  }

  /// @brief Is u an ancestor of v?
  bool is_ancestor(int u, int v) const {
    assert(ready);
    return in[u] <= in[v] and out[v] < out[u];
  }

  /// @brief Is u a descendant of v?
  bool is_descendant(int u, int v) const {
    assert(ready);
    return is_ancestor(v, u);
  }

  // parent of u when the tree is rooted at v
  int rooted_parent(int u, int v) const {
    assert(ready);
    if (root_[u] != root_[v])
      return -1;
    if (u == v) {
      return u;
    }
    if (!is_ancestor(u, v)) {
      return pv[u];
    }
    // v是u的后代
    auto it = std::upper_bound(g[u].begin(), g[u].end(), v,
                               [&](int x, std::pair<int, int> y) {
                                 return in[x] < in[y.first];
                               })
        - 1;
    return it->first;
  }
  // size of subtree u when the tree is rooted at v
  int rooted_size(int u, int v) const {
    assert(ready);
    if (root_[u] != root_[v])
      return -1;
    if (u == v) {
      return sz[root_[u]];
    }
    if (!is_ancestor(u, v)) {
      return sz[u];
    }
    return sz[root_[u]] - sz[rooted_parent(u, v)];
  }

  int depth(int v) const {
    assert(ready);
    assert(0 <= v and v < n);
    return depth_[v];
  }

  int root(int u) const {
    assert(ready);
    assert(0 <= u and u < n);
    assert(root_[u] != -1);
    return root_[u];
  }

  // pre-order traversal
  std::vector<int> top_down() const {
    assert(ready);
    return pre_order;
  }

  /// @brief returns a list of all vertices in reversed order of time when a
  /// vertex is discovered by the DFS. The latest discovered vertex comes first.
  std::vector<int> bottom_up() const {
    assert(ready);
    auto res = pre_order;
    std::reverse(res.begin(), res.end());
    return res;
  }

  int size(int v) const {
    assert(ready);
    return sz[v];
  }

  int parent(int v) const {
    assert(ready);
    return pv[v];
  }

  int parent_edge(int v) const {
    assert(ready);
    return pe[v];
  }

  int number(int u) const {
    assert(ready);
    return in[u];
  }

  std::pair<int, int> range(int u) const {
    assert(ready);
    return {in[u], out[u]};
  }

  bool is_bridge(int edge_id) const {
    assert(ready);
    return bridge.count(edge_id);
  }

  int bridge_end(int edge_id) const {
    assert(ready);
    auto it = bridge.find(edge_id);
    assert(it != bridge.end());
    return it->second;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DFS_FOREST_HPP_
