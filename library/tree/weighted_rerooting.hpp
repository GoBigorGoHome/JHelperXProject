//
// Created by zjs on 10/10/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_WEIGHTED_REROOTING_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_WEIGHTED_REROOTING_HPP_
#include <vector>

template<typename S, S (*e)(), S (*merge)(S, S), typename Weight,
         S (*apply)(S, int v, Weight edge)>
class weighted_rerooting {
  struct Edge {
    int from, to;
    Weight w;
  };
  int n;
  std::vector<S> subtree;
  std::vector<S> prod;
  std::vector<S> ans;
  std::vector<Edge> edge;
  std::vector<std::vector<int>> g;

  void dfs1(int u, int p) {
    S t = e();
    for (int i : g[u]) {
      int v = edge[i].from ^ edge[i].to ^ u;
      if (v == p)
        continue;
      prod[v] = t;
      dfs1(v, u);
      t = merge(t, apply(subtree[v], v, edge[i].w));
    }
    subtree[u] = t;
  }

  void dfs2(int u, int p, S init_value) { // init_value：子树u在点u只有它的父节点p一个孩子时的值
    ans[u] = merge(init_value, subtree[u]);

    S t = init_value;
    for (int i = (int) g[u].size() - 1; i >= 0; i--) {
      int j = g[u][i];
      int v = edge[j].from ^ edge[j].to ^ u;
      if (v == p)
        continue;
      dfs2(v, u, apply(merge(t, prod[v]), u, edge[j].w));
      t = merge(t, apply(subtree[v], v, edge[j].w));
    }
  }

 public:
  weighted_rerooting(int n) : n(n), subtree(n), prod(n), ans(n), g(n) {}

  void add_edge(int from, int to, Weight w = 1) {
    int id = (int) edge.size();
    edge.push_back({from, to, w});
    g[from].push_back(id);
    g[to].push_back(id);
  }

  std::vector<S> run() {
    dfs1(0, -1);
    dfs2(0, -1, e());
    return ans;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TREE_WEIGHTED_REROOTING_HPP_
