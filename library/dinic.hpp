//
// Created by zjsdu on 3/2/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DINIC_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DINIC_HPP_

#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>
#include <limits>

template<typename CapacityType> class dinic {
  struct arc {
    int to;
    CapacityType residual_capacity;
    int next;
    bool is_new;
  };
  bool prepare = false;
  int n;

  std::vector<arc> E;
  std::vector<int> head;

  void add(int u, int v, CapacityType c, bool is_new = false) {
    E.push_back({v, c, head[u], is_new});
    head[u] = (int) E.size() - 1;
  }

 public:
  explicit dinic(int n) : n(n), head(n, -1), level(n), cur(n) {}
  //! @brief 添加有向边
  void add_arc(int u, int v, CapacityType c) {
    assert(u >= 0 && u < n && v >= 0 && v < n);
    add(u, v, c);
    add(v, u, 0, true);
  }
  //! @brief 添加无向边
  void add_edge(int u, int v, CapacityType c) {
    assert(u >= 0 && u < n && v >= 0 && v < n);
    add(u, v, c);
    add(v, u, c);
  }

 private:
  std::vector<int> level;
  bool bfs(int s, int t) {
    std::fill(level.begin(), level.end(), -1);
    level[s] = 0;
    std::queue<int> que;
    que.push(s);
    while (!que.empty()) {
      int u = que.front();
      que.pop();
      for (int i = head[u]; i != -1; i = E[i].next) {
        int v = E[i].to;
        if (E[i].residual_capacity > 0 && level[v] == -1) {
          level[v] = level[u] + 1;
          if (v == t)
            return true;// 优化1
          que.push(v);
        }
      }
    }
    return false;
  }

  std::vector<int> cur;
  CapacityType dfs(int u, CapacityType capacity, int t) {
    if (u == t) {
      return capacity;
    }
    CapacityType total = 0;
    // 优化2
    for (int& i = cur[u]; i != -1; i = E[i].next) {
      int v = E[i].to;
      if (level[u] < level[v] && E[i].residual_capacity > 0) {
        CapacityType tmp =
            dfs(v, std::min(capacity, E[i].residual_capacity), t);
        if (tmp == 0)
          continue;
        E[i].residual_capacity -= tmp;
        E[i xor 1].residual_capacity += tmp;
        total += tmp;
        capacity -= tmp;
        if (capacity == 0)
          break;
      }
    }
    return total;
  }

 public:
  CapacityType max_flow(int source, int sink) {
    CapacityType ans = 0;
    while (bfs(source, sink)) {
      cur = head;
      ans += dfs(source, std::numeric_limits<CapacityType>::max(), sink);
    }
    prepare = true;
    return ans;
  }

  std::vector<std::pair<int, int>> min_cut() {
    assert(prepare);
    std::vector<std::pair<int, int>> cut;
    for (int u = 0; u < n; u++) {
      if (level[u] == -1)
        continue;
      for (int i = head[u]; i != -1; i = E[i].next) {
        if (!E[i].is_new && level[E[i].to] == -1) {
          cut.emplace_back(u, E[i].to);
        }
      }
    }
    return cut;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DINIC_HPP_
