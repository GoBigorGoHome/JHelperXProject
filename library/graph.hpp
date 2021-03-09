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

 public:
  explicit graph(int n) : n(n), g(n) { assert(n > 0); }
  /// @return Number of vertices.
  int v_size() const { return n; }
  virtual void add_edge(int u, int v) = 0;
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
