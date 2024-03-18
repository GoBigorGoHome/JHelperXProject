//
// Created by zjs on 6/20/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_REROOTING_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_REROOTING_TREE_HPP_

#include <vector>

// Node：表示子树的信息的类型。
template<typename Node, Node (*add_child)(Node, Node),
         Node (*remove_child)(Node, Node)>
class rerooting_tree {
  std::vector<std::vector<int>> adj;
  std::vector<Node> node;

  void dfs(int u, int p) {
    for (int v : adj[u]) {
      if (v == p)
        continue;
      dfs(v, u);
      node[u] = add_child(node[u], node[v]);
    }
  }

  void do_reroot(int u, int p) {
    for (int v : adj[u]) {
      if (v == p)
        continue;
      node[v] = add_child(node[v], remove_child(node[u], node[v]));
      do_reroot(v, u);
    }
  }

 public:
  rerooting_tree(int n) : adj(n), node(n) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  std::vector<Node> work() {
    dfs(0, -1);
    do_reroot(0, -1);
    return node;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_REROOTING_TREE_HPP_
