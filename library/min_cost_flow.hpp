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
    Cap cap;
    Cost cost;
  };

  int n;
  std::vector<int> head;
  std::vector<arc> e;
  const Cost INF_COST = std::numeric_limits<Cost>::max();
  std::vector<Cost> d;

  Cost dijkstra(int s, int t) {
    std::fill(d.begin(), d.end(), INF_COST);
    using P = std::pair<Cost, int>;
    std::priority_queue<P, std::vector<P>, std::greater<>> q;
    q.emplace(d[s] = 0, s);
    while (!q.empty()) {
      auto top = q.top();
      q.pop();
      // 由于有负权边，当d[u]首次作为最小值时，d[u]未必是s到u的最短路长度。
      if (top.second == t)
        continue;
      if (d[top.second] != top.first)
        continue;
      for (int i = head[top.second]; i != -1; i = e[i].next) {
        int v = e[i].to;
        if (e[i].cap > 0 && d[v] > top.first + e[i].cost) {
          d[v] = top.first + e[i].cost;
          q.emplace(d[v], v);
        }
      }
    }
    return d[t];
  }

  std::vector<bool> vis;
  std::vector<int> cur;

  Cap dfs(int u, Cap flow, int t) {
    if (u == t)
      return flow;
    // 图中可能有边权之和为零的环，所以必须要有vis标记，不然dfs寻路时可能绕着边权之和为零的环一直转。
    vis[u] = true;
    Cap pushed = 0;
    for (int& i = cur[u]; i != -1; i = e[i].next) {
      int v = e[i].to;
      if (not vis[v] and e[i].cap and d[v] == d[u] + e[i].cost) {
        if (Cap f = dfs(v, std::min(flow, e[i].cap), t)) {
          e[i].cap -= f;
          e[i ^ 1].cap += f;
          pushed += f;
          flow -= f;
          if (flow == 0)
            break;
        }
      }
    }
    vis[u] = false;
    return pushed;
  }

 public:
  explicit MinCostFlow(int n) : n(n), head(n, -1), d(n), vis(n) {}

  void add_arc(int u, int v, Cap cap, Cost cost) {
    e.push_back({v, head[u], cap, cost});
    head[u] = (int) e.size() - 1;
    e.push_back({u, head[v], 0, -cost});
    head[v] = (int) e.size() - 1;
  }

  std::pair<Cap, Cost> min_cost_flow(int s, int t) {
    Cost cost = 0;
    Cap flow = 0;
    while (dijkstra(s, t) < 0) {
      cur = head;
      Cap f = dfs(s, std::numeric_limits<Cap>::max(), t);
      cost += f * d[t];
    }
    return {flow, cost};
  }

  std::pair<Cap, Cost> min_cost_max_flow(int s, int t) {
    Cost cost = 0;
    Cap flow = 0;
    while (dijkstra(s, t) != INF_COST) {
      cur = head;
      Cap f = dfs(s, std::numeric_limits<Cap>::max(), t);
      cost += f * d[t];
    }
    return {flow, cost};
  }

  std::vector<Cost> slope(int s, int t) {
    Cost cost = 0;
    std::vector<Cost> ans{0};
    while (dijkstra(s, t) != INF_COST) {
      cur = head;
      Cap f = dfs(s, std::numeric_limits<Cap>::max(), t);
      for (Cap i = 0; i < f; i++) {
        cost += d[t];
        ans.push_back(cost);
      }
    }
    return ans;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MIN_COST_FLOW_HPP_
