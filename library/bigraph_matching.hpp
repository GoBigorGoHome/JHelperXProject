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
template<typename Graph>
std::vector<int> max_matching(const Graph &g, unsigned R) {
  auto L = g.size();
  std::vector<bool> bad(L);
  std::vector<int> match(R, -1), time_stamp(R, -1), order(R), low(R);
  std::vector<bool> in_stack(R), excluded(R);
  int now = 0;
  auto augment = [&](int u) {
    int time = 0;
    auto dfs_impl = [&](int u, const auto &dfs) -> int {
      int low_value = INT_MAX;
      for (int v : g[u]) {
        if (match[v] == u or excluded[v])
          continue;
        if (time_stamp[v] == -1) {
          time_stamp[v] = now;
          match[v] = u;
          return -1;
        } else if (time_stamp[v] < now) {
          if (bad[time_stamp[v]])
            continue;
          time_stamp[v] = now;
          order[v] = low[v] = time;
          in_stack[time] = true;
          ++time;
          auto ret = dfs(match[v], dfs);
          in_stack[order[v]] = false;
          if (ret == -1) {
            match[v] = u;
            return -1;
          } else {
            low_value = std::min(low_value, ret);
            low[v] = std::min(low[v], ret);
            if (low[v] == order[v]) {
              excluded[v] = true;
            }
          }
        } else {
          if (in_stack[order[v]]) {
            low_value = std::min(low_value, order[v]);
          } else if (in_stack[low[v]]) {
            low_value = std::min(low_value, low[v]);
          } else {
            excluded[v] = true;
          }
        }
      }
      return low_value;
    };
    // Search for an augmenting path from u.
    return dfs_impl(u, dfs_impl) == -1;
  };
  for (int u = 0; u < L; ++u) {
    if (not augment(u))
      bad[now] = true;
    ++now;
  }
  return match;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BIGRAPH_MATCHING_HPP_
