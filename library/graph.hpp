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
  std::vector<std::vector<std::pair<int, int>>> g;
  int eid_counter = 0;

 public:
  explicit graph(int n) : n(n), g(n) { assert(n >= 0); }
  /// @return Number of vertices.
  int v_size() const { return n; }

  void add_edge(int u, int v, int edge_id) {
    assert(0 <= u and u < n and 0 <= v and v < n);
    assert(edge_id >= 0);
    g[u].push_back({v, edge_id});
    g[v].push_back({u, edge_id});
  }

  void add_edge(int u, int v) {
    assert(0 <= u and u < n and 0 <= v and v < n);
    g[u].push_back({v, eid_counter});
    g[v].push_back({u, eid_counter});
    eid_counter++;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
