//
// Created by zjs on 2/22/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_VIRTUAL_TREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_VIRTUAL_TREE_HPP_

#include "lca_tree.hpp"
#include <algorithm>
//虚树是对 virtual tree 的翻译。virtual tree 通称 auxiliary tree。
struct virtual_tree : lca_tree {
  virtual_tree(int n) : lca_tree(n) {}
  /// @param key_nodes 关键节点
  /// @param vtree 用来存储虚树
  /// @param is_sorted 关键节点是否已经按 DFS 序从小到大排好序
  /// @returns 虚树的根节点
  int build_virtual_tree(std::vector<int> key_nodes,
                         std::vector<std::vector<int>> &vtree,
                         bool is_sorted = false) {
    assert(!key_nodes.empty());
    assert(ready);
    if (!is_sorted) {
      std::sort(key_nodes.begin(), key_nodes.end(),
                [&](int u, int v) { return num[u] < num[v]; });
    }

    std::vector<int> right_path;//当前构建的虚树中最右边的一条链。
    auto push = [&](int v) {
      right_path.push_back(v);
      vtree[v].clear();
    };
    // 构建虚树
    for (int u : key_nodes) {
      if (!right_path.empty()) {
        int w = lca(u, right_path.back());
        // 下面的 while 循环做的事情：
        // right_path 里最后那一批，是 w 的严格后代的，节点，它们在虚树里的父节点已经确定。
        // 把这样的节点从 right_path 里移除。
        while (num[w] < num[right_path.back()]) {
          int v = right_path.back();
          right_path.pop_back();
          if (right_path.empty() || num[right_path.back()] < num[w])
            push(w);
          // 在虚树里v的父节点就是right_path.back()
          vtree[right_path.back()].push_back(v);
        }
      }
      push(u);
    }

    for (int i = 0; i < (int) right_path.size() - 1; i++)
      vtree[right_path[i]].push_back(right_path[i + 1]);

    return right_path[0]; // 返回虚树的根
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_VIRTUAL_TREE_HPP_
