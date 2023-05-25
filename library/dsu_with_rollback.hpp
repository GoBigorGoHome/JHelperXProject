//
// Created by zjs on 2023/2/9.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DSU_WITH_ROLLBACK_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DSU_WITH_ROLLBACK_HPP_

#include <vector>
#include <cassert>
#include <tuple>
#include <algorithm>

class dsu_with_rollback {
  std::vector<int> ps;// parent or size
  std::vector<bool> is_tree;
  std::vector<std::tuple<int, int, bool>> record;// merge record
  int treeCnt;// number of connected components that are trees
  int n;

 public:
  explicit dsu_with_rollback(int n)
      : ps(n, -1), is_tree(n, true), treeCnt(n), n(n) {}

  void clear() {
    std::fill(ps.begin(), ps.end(), -1);
    std::fill(is_tree.begin(), is_tree.end(), true);
    record.clear();
    treeCnt = n;
  }

  int root(int u) const {
    while (ps[u] >= 0)
      u = ps[u];
    return u;
  }

  int size(int u) const { return -ps[root(u)]; }

  int tree_cnt() const { return treeCnt; }

  int edge_cnt() const { return (int) record.size(); }

  bool merge(int u, int v) {
    u = root(u);
    v = root(v);
    if (u == v) {
      record.push_back({u, 0, is_tree[u]});
      treeCnt -= is_tree[u];
      is_tree[u] = false;
      return false;
    }
    if (ps[u] < ps[v])
      std::swap(u, v);
    record.push_back({u, ps[u], is_tree[v]});
    ps[v] += ps[u];
    ps[u] = v;
    if (is_tree[u] || is_tree[v])
      treeCnt--;
    if (is_tree[u] != is_tree[v])
      is_tree[v] = false;
    return true;
  }

  void rollback() {
    assert(!record.empty());
    auto [u, sz, wasTree] = record.back();
    record.pop_back();

    if (sz == 0) {
      is_tree[u] = wasTree;
      if (is_tree[u])
        treeCnt++;
    } else {
      int v = ps[u];
      ps[u] = sz;
      ps[v] -= sz;
      is_tree[v] = wasTree;
      if (is_tree[u] || is_tree[v])
        treeCnt++;
    }
  }

  void rollback_to(int edge_cnt) {
    assert(edge_cnt >= 0);
    while ((int) record.size() > edge_cnt)
      rollback();
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DSU_WITH_ROLLBACK_HPP_
