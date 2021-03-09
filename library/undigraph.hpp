//
// Created by zjsdu on 3/5/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_UNDIGRAPH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_UNDIGRAPH_HPP_
#include <graph.hpp>
class undigraph : public graph {
  struct edge {
    int u, v;
    bool enabled = true;
  };
  int m = 0;
  int m_enabled = 0;
  std::vector<int> deg;
  std::vector<edge> edges;

 public:
  explicit undigraph(int n) : graph(n), deg(n) {}
  /// @return Number of edges.
  int e_size() const { return m; }
  /// @return Number of enabled edges.
  int e_size_enabled() const { return m_enabled; }
  void add_edge(int u, int v) override {
    int id = (int) edges.size();
    g[u].push_back(id);
    g[v].push_back(id);
    edges.push_back({u, v});
    ++deg[u];
    ++deg[v];
    ++m;
    ++m_enabled;
  }
  void enable_edge(int i) {
    if (not edges[i].enabled) {
      edges[i].enabled = true;
      ++m_enabled;
      ++deg[edges[i].u];
      ++deg[edges[i].v];
    }
  }
  void disable_edge(int i) {
    if (edges[i].enabled) {
      edges[i].enabled = false;
      --m_enabled;
      --deg[edges[i].u];
      --deg[edges[i].v];
    }
  }
  /// @return A list of IDs of all the edges at a vertex u.
  const std::vector<int> &edges_at(int u) const {
    assert(0 <= u and u < n);
    return g[u];
  }
  const edge &edge(int i) const { return edges[i]; }
  int degree(int i) const { return deg[i]; }
  bool is_enabled(int i) const { return edges[i].enabled; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_UNDIGRAPH_HPP_
