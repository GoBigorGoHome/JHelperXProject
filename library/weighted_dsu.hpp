//
// Created by zjs on 2023/5/16.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_WEIGHTED_DSU_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_WEIGHTED_DSU_HPP_
#include <vector>
// union by size, edge has weight_
template<typename T> class weighted_dsu {
  std::vector<int> ps;// parent or size
  std::vector<T> weight_;

 public:
  explicit weighted_dsu(int n) : ps(n, -1), weight_(n) {}

  T weight(int u) const { return weight_[u]; }

  int root(int u) const {
    while (ps[u] >= 0)
      u = ps[u];
    return u;
  }

  bool same(int u, int v) const { return root(u) == root(v); }

  bool merge(int u, int v, T w) {
    u = root(u);
    v = root(v);
    if (u == v)
      return false;
    if (ps[u] > ps[v])
      std::swap(u, v);
    // ps[u] <= ps[v]
    ps[u] += ps[v];
    ps[v] = u;
    weight_[v] = w;
    return 1;
  }

  int lca(int u, int v) const {
    if (u == v)
      return u;
    std::vector<int> path_u, path_v;
    while (u >= 0) {
      path_u.push_back(u);
      u = ps[u];
    }
    while (v >= 0) {
      path_v.push_back(v);
      v = ps[v];
    }

    if (path_u.back() != path_v.back())
      return -1;

    int i = (int) path_u.size() - 1;
    int j = (int) path_v.size() - 1;
    while (i >= 0 && j >= 0 && path_u[i] == path_v[j]) {
      i--;
      j--;
    }
    return path_u[i + 1];
  }

  std::pair<std::vector<int>, std::vector<int>> path_to_lca(int u,
                                                            int v) const {
    std::vector<int> path_u, path_v;
    while (u >= 0) {
      path_u.push_back(u);
      u = ps[u];
    }
    while (v >= 0) {
      path_v.push_back(v);
      v = ps[v];
    }
    if (path_u.back() != path_v.back())
      return {};
    int i = (int) path_u.size() - 1;
    int j = (int) path_v.size() - 1;
    while (i >= 0 && j >= 0 && path_u[i] == path_v[j]) {
      i--;
      j--;
    }
    return {std::vector<int>(path_u.begin(), path_u.begin() + i + 1),
            std::vector<int>(path_v.begin(), path_v.begin() + j + 1)};
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_WEIGHTED_DSU_HPP_
