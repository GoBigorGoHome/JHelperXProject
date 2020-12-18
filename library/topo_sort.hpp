//
// Created by zjsdu on 11/3/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TOPO_SORT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TOPO_SORT_HPP_
#include <vector>
#include <algorithm>
template<typename G> std::vector<int> topo_sort(const G &g) {
  std::vector<int> order;
  order.reserve(g.size());
  std::vector<int> vis(g.size());
  auto dfs = [&](int u) {
    auto dfs_impl = [&](int u, const auto &dfs) -> bool {
      vis[u] = -1;
      for (int v : g[u])
        if (not vis[v]) {
          bool ret = dfs(v, dfs);
          if (not ret)
            return false;
        } else if (vis[v] == -1)
          return false;
      vis[u] = 1;
      order.push_back(u);
      return true;
    };
    return dfs_impl(u, dfs_impl);
  };
  for (int i = 0; i < (int) g.size(); ++i)
    if (not vis[i] and not dfs(i))
      return {};
  std::reverse(order.begin(), order.end());
  return order;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TOPO_SORT_HPP_
