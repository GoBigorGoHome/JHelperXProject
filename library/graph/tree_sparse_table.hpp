//
// Created by zjs on 2/23/25.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_TREE_SPARSE_TABLE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_TREE_SPARSE_TABLE_HPP_
#include <vector>
#include <cassert>
#include <algorithm>
template<typename T, typename F>
struct tree_sparse_table {
  int n;
  std::vector<std::vector<int>> g;
  T e;
  F func;
  std::vector<std::vector<T>> mat;
  int h;
  std::vector<int> depth;
  std::vector<std::vector<int>> anc;
  /// @param a value array, a[i] should be the value of node i.
  /// @param e identity element
  /// @param f function object, signature T f(T,T), f shoule be commutative.
  tree_sparse_table(const std::vector<T> &a, T e, F f)
      : n((int) a.size()), g(n), e(e), func(f), mat(1) {
    mat[0] = a;
  }

  void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  }

  void build() {
    depth.resize(n);
    anc.resize(1);
    anc[0].resize(n);
    dfs(0, -1);
    int max_depth = *std::max_element(depth.begin(), depth.end());

    h = 1;
    while (1 << h <= max_depth)
      h++;
    mat.resize(h);
    anc.resize(h);
    for (int i = 1; i < h; i++) {
      mat[i].resize(n);
      anc[i].resize(n, -1);
      for (int j = 0; j < n; j++) {
        int k = anc[i - 1][j];
        if (k == -1)
          continue;
        anc[i][j] = anc[i - 1][k];
        if (anc[i][j] != -1)
          mat[i][j] = func(mat[i - 1][j], mat[i - 1][k]);
      }
    }
  }

  T prod(int u, int v, bool with_lca) const {
    T res = e;
    if (depth[u] < depth[v])
      std::swap(u, v);
    int up = depth[u] - depth[v];
    for (int i = 0; i < h; i++)
      if (up >> i & 1) {
        res = func(res, mat[i][u]);
        u = anc[i][u];
      }
    int w;
    if (u == v)
      w = u;
    else {
      for (int i = h - 1; i >= 0; i--)
        if (anc[i][u] != anc[i][v]) {
          res = func(res, mat[i][u]);
          res = func(res, mat[i][v]);
          u = anc[i][u];
          v = anc[i][v];
        }
      res = func(res, mat[0][u]);
      res = func(res, mat[0][v]);
      w = anc[0][u];
    }
    if (with_lca)
      res = func(res, mat[0][w]);
    return res;
  }

 private:
  void dfs(int u, int p) {
    anc[0][u] = p;
    for (int v : g[u])
      if (v != p) {
        depth[v] = depth[u] + 1;
        dfs(v, u);
      }
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_TREE_SPARSE_TABLE_HPP_
