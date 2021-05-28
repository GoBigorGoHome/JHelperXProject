//
// Created by zjsdu on 2/20/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_HPP_
#include <graph.hpp>
class tree : public graph {
 public:
  tree(int n) : graph(n) {}
  void add_edge(int u, int v) override {
    assert(0 <= u and u < n and 0 <= v and v < n);
    g[u].push_back(v);
    g[v].push_back(u);
  }
  const std::vector<int> &neighbors(int u) const { return g[u]; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_HPP_
