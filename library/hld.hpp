//
// Created by zjs on 2022/3/13.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_HLD_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_HLD_HPP_
#include <vector>
#include <algorithm>

namespace HLD {

std::vector<int> parent, depth, heavy, head;

int dfs(int u, int p, const std::vector<std::vector<int>>& adj) {
  parent[u] = p;
  int size = 1;
  int max_c_size = 0, c = -1;
  for (int v : adj[u])
    if (v != p) {
      depth[v] = depth[u] + 1;
      int c_size = dfs(v, u, adj);
      size += c_size;
      if (c_size > max_c_size) {
        max_c_size = c_size;
        c = v;
      }
    }
  heavy[u] = c;
  return size;
}

void build(const std::vector<std::vector<int>>& adj) {
  int n = (int) adj.size();
  parent.assign(n, -1);
  heavy.assign(n, -1);
  head.assign(n, 0);
  depth.assign(n, 0);
  dfs(0, -1, adj);
  for (int i = 0; i < n; i++)
    if (i == 0 or heavy[parent[i]] != i)
      for (int j = i; j != -1; j = heavy[j])
        head[j] = i;
}

int lca(int u, int v) {
  while (head[u] != head[v]) {
    if (depth[head[u]] > depth[head[v]])
      u = parent[head[u]];
    else
      v = parent[head[v]];
  }
  return depth[u] < depth[v] ? u : v;
}

int lca_depth(int u, int v) {
  while (head[u] != head[v]) {
    if (depth[head[u]] > depth[head[v]])
      u = parent[head[u]];
    else
      v = parent[head[v]];
  }
  return std::min(depth[u], depth[v]);
}

}// namespace HLD
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_HLD_HPP_
