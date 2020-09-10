//
// Created by zjsdu on 9/10/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BIGRAPH_MATCHING_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BIGRAPH_MATCHING_HPP_
#include <vector>
#include <numeric>
#include <algorithm>
/// @brief Find a maximum cardinality matching in a bipartite graph.
/// @param g Bipartite graph with edges directed from left part to right part.
/// @pre Nodes in both left and right parts are 0-indexed.
/// @param R Number of nodes in the right part of g.
/// @return An array match[0..R) where match[i] is the node that the i-th node
/// in the right part is matched with.
std::vector<int> max_matching(const std::vector<std::vector<int>> &g,
                              unsigned R) {
  int L = (int) g.size();
  std::vector<int> order(L);
  std::iota(order.begin(), order.end(), 0);
  std::sort(order.begin(), order.end(),
            [&g](int u, int v) { return g[u].size() < g[v].size(); });
  std::vector<bool> bad(L);
  std::vector<int> match(R, -1), time_stamp(R, -1);
  int now = 0;
  auto augment = [&](int u) {
    auto dfs_impl = [&](int u, auto dfs) -> bool {
      for (int v : g[u])
        if (time_stamp[v] == -1) {
          time_stamp[v] = now;
          match[v] = u;
          return true;
        } else if (time_stamp[v] < now and not bad[time_stamp[v]]) {
          time_stamp[v] = now;
          if (dfs(match[v], dfs)) {
            match[v] = u;
            return true;
          }
        }
      return false;
    };
    // search augmenting path from u
    return dfs_impl(u, dfs_impl);
  };
  for (int u : order) {
    if (not augment(u))
      bad[now] = true;
    ++now;
  }
  return match;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BIGRAPH_MATCHING_HPP_
