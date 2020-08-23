//
// Created by zjsdu on 8/21/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_2_EDGE_CONNECTED_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_2_EDGE_CONNECTED_HPP_
#include <vector>
#include <tuple>
/// @brief Calculate 2-edge-connected components of a connected undirected
/// graph.
/// @param g Connected undirected graph, represented as adjacent lists of tuples
/// (to_vertex, edge_index).
/// @param keep_bridge_ends If false, use 2-edge-connected component ID as
/// bridge ends.
/// @returns component ID of each vertex; bridges in bottom-up order.
std::pair<std::vector<int>, std::vector<std::tuple<int, int, int>>>
two_edge_connected_components(
    const std::vector<std::vector<std::pair<int, int>>> &g,
    bool keep_bridge_ends = false) {
  size_t n = g.size();
  std::vector<int> component_id(n);
  // low[u]: the minimum depth that can be reached from u by travelling down
  // some (possibly zero) tree edges and then a back edge.
  std::vector<int> low(n);
  std::vector<int> depth(n);
  std::vector<std::tuple<int, int, int>> bridges;
  std::vector<int> stack;
  int component_cnt = 0;
  auto impl = [&](int u, int fa, auto &dfs) -> void {
    low[u] = depth[u] = depth[fa] + 1;
    stack.push_back(u);
    for (auto [v, i] : g[u]) {
      if (v == fa)
        continue;
      if (depth[v] == 0)
        low[u] = std::min(low[u], depth[v]);
      else {
        dfs(v, u, dfs);
        low[u] = std::min(low[u], low[v]);
        if (low[v] == depth[v]) {
          bridges.emplace_back(u, v, i);
          while (true) {
            int x = stack.back();
            stack.pop_back();
            component_id[x] = component_cnt;
            if (x == v)
              break;
          }
          ++component_cnt;
        }
      }
    }
  };
  impl(0, 0, impl);
  for (int u : stack)
    component_id[u] = component_cnt;
  if (not keep_bridge_ends)
    for (auto &e : bridges) {
      std::get<0>(e) = component_id[std::get<0>(e)];
      std::get<1>(e) = component_id[std::get<1>(e)];
    }
  return {component_id, bridges};
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_2_EDGE_CONNECTED_HPP_
