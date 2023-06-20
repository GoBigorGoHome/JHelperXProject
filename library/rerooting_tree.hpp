//
// Created by zjs on 6/20/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_REROOTING_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_REROOTING_TREE_HPP_
#include <Graph.hpp>
template<typename Contrib, typename Node, Contrib (*get_contrib)(Node),
         void (*add_child)(Node &, Contrib),
         Contrib (*remove_child)(Node, Node)>
class rerooting_tree : public Graph {
  std::vector<Node> node;
  void dfs(int u, int p) {
    for (int v : adj[u]) {
      if (v == p)
        continue;
      dfs(v, u);
      add_child(node[u], get_contrib(node[v]));
    }
  }

  void do_reroot(int u, int p) {
    for (int v : adj[u]) {
      if (v == p)
        continue;
      add_child(node[v], remove_child(node[u], node[v]));
      do_reroot(v, u);
    }
  }

 public:
  rerooting_tree(int n) : Graph(n), node(n) {}
  std::vector<Node> work() {
    dfs(0, -1);
    do_reroot(0, -1);
    return node;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_REROOTING_TREE_HPP_
