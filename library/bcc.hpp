//
// Created by zjs on 9/6/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BCC_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BCC_HPP_
#include <vector>
#include <algorithm>
#include <cassert>
// 计算无向图的（点）双连通分量
// 重边和自环对于图的双连通性无意义，所以一般假设图是简单无向图。不过这个代码也能处理有重边或自环的图。
// 例题：abc318g Typical Path Problem
class bcc_graph {
  int n, m;
  std::vector<int> bcc_id;
  std::vector<int> num;
  std::vector<std::vector<std::pair<int, int>>> g;
  std::vector<int> low;
  std::vector<bool> is_articulation_;
  std::vector<bool> vis;
  std::vector<int> path_;
  std::vector<int> stk;// 存放边的栈
  int dfs_num;
  int bcc_cnt;

  void biconnect(int u, int p) {
    num[u] = low[u] = ++dfs_num;
    int children_cnt = 0;
    for (auto [v, i] : g[u]) {
      if (!num[v]) {
        children_cnt++;
        stk.push_back(i);
        biconnect(v, u);
        low[u] = std::min(low[u], low[v]);
        if (low[v] >= num[u]) {
          bcc_cnt++;
          int j;
          do {
            j = stk.back();
            stk.pop_back();
            bcc_id[j] = bcc_cnt;
          } while (j != i);
          if (p != -1) {// 点u不是根
            is_articulation_[u] = true;
          }
        }
      } else if (v != p && num[v] < num[u]) {
        stk.push_back(i);
        low[u] = std::min(low[u], num[v]);
      }
    }
    if (p == -1 && children_cnt > 1)
      is_articulation_[u] = 1;
  }

  bool find_path(int u, int t) {
    vis[u] = true;
    if (u == t)
      return true;
    for (auto [v, i] : g[u])
      if (!vis[v] && find_path(v, t)) {
        if (path_.empty() || bcc_id[i] != path_.back())
          path_.push_back(bcc_id[i]);
        return true;
      }
    return false;
  }

 public:
  bcc_graph(int n)
      : n(n), m(0), num(n), g(n), low(n), is_articulation_(n), vis(n) {}

  void add_edge(int u, int v) {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    g[u].push_back({v, m});
    g[v].push_back({u, m});
    m++;
  }

  void bcc() {
    bcc_cnt = 0;
    dfs_num = 0;
    bcc_id.assign(m, 0);
    for (int i = 0; i < n; i++)
      if (!num[i])
        biconnect(i, -1);
  }
  // 从s到t的一条简单路径上的边所属的双连通分量。
  std::vector<int> path(int s, int t) {
    std::fill(vis.begin(), vis.end(), false);
    path_.clear();
    find_path(s, t);
    std::reverse(path_.begin(), path_.end());
    return path_;
  }
  // 点u是否为割点？
  bool is_articulation_point(int u) const { return is_articulation_[u]; }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BCC_HPP_
