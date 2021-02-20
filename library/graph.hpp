//
// Created by zjsdu on 2/19/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#include <vector>
class graph {
 protected:
  const int n;
  std::vector<std::vector<int>> g;

 public:
  explicit graph(int n) : n(n), g(n) { assert(n > 0); }
  virtual void add_edge(int u, int v) = 0;
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
