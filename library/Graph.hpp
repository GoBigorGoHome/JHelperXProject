//
// Created by zjs on 6/20/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#include <vector>
class Graph {
 protected:
  std::vector<std::vector<int>> adj;

 public:
  explicit Graph(int n) : adj(n) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
