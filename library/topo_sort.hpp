//
// Created by zjsdu on 11/3/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TOPO_SORT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TOPO_SORT_HPP_
#include <vector>
#include <algorithm>
std::vector<int> topo_sort(const std::vector<std::vector<int>> &g) {
  std::vector<int> order;
  order.reserve(g.size());
  std::vector<int> vis(g.size());
  auto dfs = [&](int u, const auto &self) {
    if (vis[u] == -1)
      return 0;
    if (vis[u] == 1)
      return 1;
    vis[u] = -1;
    for (int v : g[u])
      if (!self(v, self))
        return 0;
    vis[u] = 1;
    order.push_back(u);
    return 1;
  };
  for (int i = 0; i < (int) g.size(); ++i)
    if (!dfs(i, dfs))
      return {};
  std::reverse(order.begin(), order.end());
  return order;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TOPO_SORT_HPP_
