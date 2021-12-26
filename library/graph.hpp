//
// Created by zjsdu on 2/19/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#include <vector>
#include <cassert>
class graph {
 protected:
  const int n;
  std::vector<std::vector<int>> g;
  std::vector<std::pair<int, int>> edges;

 public:
  explicit graph(int n) : n(n), g(n) { assert(n > 0); }
  /// @return Number of vertices.
  int v_size() const { return n; }
  int add_edge(int u, int v) {
    assert(0 <= u and u < n and 0 <= v and v < n);
    int id = (int) edges.size();
    edges.emplace_back(u, v);
    g[u].push_back(id);
    g[v].push_back(id);
    return id;
  }

  std::pair<int, int> edge(int id) const {
    assert(0 <= id and id < (int) edges.size());
    return edges[id];
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
