//
// Created by zjsdu on 11/11/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MIN_COST_FLOW_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MIN_COST_FLOW_HPP_
#include <vector>
#include <queue>
#include <algorithm>

template<typename Cap, typename Cost> class MinCostFlow {
  struct arc {
    int to, next;
    Cost cost;
    Cap cap;
  };

  int n;
  std::vector<int> head;
  std::vector<arc> e;
  const Cost INF_COST = std::numeric_limits<Cost>::max();
  std::vector<Cost> d;

  bool dijkstra(int s, int t) {
    std::fill(d.begin(), d.end(), INF_COST);
    std::priority_queue<std::pair<Cost, int>, std::vector<std::pair<Cost, int>>,
                        std::greater<>>
        que;
    que.push({d[s] = 0, s});
    while (!que.empty()) {
      auto top = que.top();
      que.pop();
      if (top.second == t)
        return true;
      if (d[top.second] != top.first)
        continue;
      for (int i = head[top.second]; i != -1; i = e[i].next) {
        int v = e[i].to;
        if (e[i].cap > 0 && d[v] > top.first + e[i].cost) {
          d[v] = top.first + e[i].cost;
          que.push({d[v], v});
        }
      }
    }
    return d[t] != INF_COST;
  }

  std::vector<int> cur;

  Cap dfs(int u, Cap cap, int t) {
    if (u == t)
      return cap;
    Cap pushed = 0;
    for (int& i = cur[u]; i != -1; i = e[i].next) {
      int v = e[i].to;
      if (e[i].cap and d[v] == d[u] + e[i].cost) {
        if (Cap f = dfs(v, std::min(cap, e[i].cap), t)) {
          e[i].cap -= f;
          e[i ^ 1].cap += f;
          pushed += f;
          cap -= f;
          if (cap == 0)
            break;
        }
      }
    }
    return pushed;
  }

 public:
  explicit MinCostFlow(int n) : n(n), head(n, -1), d(n) {}

  void add_arc(int u, int v, int cost, int cap) {
    e.push_back({v, head[u], cost, cap});
    head[u] = (int) e.size() - 1;
    e.push_back({u, head[v], -cost, 0});
    head[v] = (int) e.size() - 1;
  }

  Cap min_cost_flow(int s, int t) {
    Cap ans = 0;
    while (dijkstra(s, t)) {
      cur = head;
      while (int flow = dfs(s, INT_MAX, t))
        ans += flow * d[t];
    }
    return ans;
  }

  std::vector<Cost> slope(int s, int t) {
    Cap flow = 0;
    Cost cost = 0;
    std::vector<Cost> ans{0};
    while (dijkstra(s, t)) {
      cur = head;
      Cap f = dfs(s, std::numeric_limits<Cap>::max(), t);
      for (Cap i = 1; i <= flow; i++) {
        cost += d[t];
        ans.push_back(cost);
      }
    }
    return ans;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MIN_COST_FLOW_HPP_
