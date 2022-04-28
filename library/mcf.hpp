//
// Created by zjs on 2022/4/26.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MCF_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MCF_HPP_

#include <numeric>
#include <vector>
#include <queue>
#include <cassert>

// copy from https://atcoder.jp/contests/abc214/submissions/25655008
// author: hitonanode

// Minimum cost flow WITH NO NEGATIVE CYCLE (just negative cost edge is allowed)
// Verified:
// - SRM 770 Div1 Medium
// https://community.topcoder.com/stat?c=problem_statement&pm=15702
// - CodeChef LTIME98 Ancient Magic https://www.codechef.com/problems/ANCT
template<class Cap = long long, class Cost = long long,
         Cost INF_COST = std::numeric_limits<Cost>::max() / 2>
struct MinCostFlow {
  struct _edge {
    int to, rev;
    Cap cap;
    Cost cost;
  };
  bool _is_dual_infeasible;
  int V;
  std::vector<std::vector<_edge>> g;
  std::vector<Cost> dist;
  std::vector<int> prevv, preve;
  std::vector<Cost> dual;// dual[V]: potential
  std::vector<std::pair<int, int>> pos;

  bool _initialize_dual_dag() {
    std::vector<int> deg_in(V);
    for (int i = 0; i < V; i++) {
      for (const auto& e : g[i])
        deg_in[e.to] += (e.cap > 0);
    }
    std::vector<int> st;
    st.reserve(V);
    for (int i = 0; i < V; i++) {
      if (!deg_in[i])
        st.push_back(i);
    }
    for (int n = 0; n < V; n++) {
      if (int(st.size()) == n)
        return false;// Not DAG
      int now = st[n];
      for (const auto& e : g[now]) {
        if (!e.cap)
          continue;
        deg_in[e.to]--;
        if (deg_in[e.to] == 0)
          st.push_back(e.to);
        if (dual[e.to] >= dual[now] + e.cost)
          dual[e.to] = dual[now] + e.cost;
      }
    }
    return true;
  }

  bool _initialize_dual_spfa() {// Find feasible dual's when negative cost edges
                                // exist
    dual.assign(V, 0);
    std::queue<int> q;
    std::vector<int> in_queue(V);
    std::vector<int> nvis(V);
    for (int i = 0; i < V; i++)
      q.push(i), in_queue[i] = true;
    while (q.size()) {
      int now = q.front();
      q.pop(), in_queue[now] = false;
      if (nvis[now] > V)
        return false;// Negative cycle exists
      nvis[now]++;
      for (const auto& e : g[now]) {
        if (!e.cap)
          continue;
        if (dual[e.to] > dual[now] + e.cost) {
          dual[e.to] = dual[now] + e.cost;
          if (!in_queue[e.to])
            in_queue[e.to] = true, q.push(e.to);
        }
      }
    }
    return true;
  }

  bool initialize_dual() {
    return !_is_dual_infeasible or _initialize_dual_dag()
        or _initialize_dual_spfa();
  }

  void _dijkstra(int s) {// O(ElogV)
    prevv.assign(V, -1);
    preve.assign(V, -1);
    dist.assign(V, INF_COST);
    dist[s] = 0;
    typedef typename std::make_unsigned<Cost>::type UCost;
    using P = std::pair<UCost, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> q;
    q.emplace(0, s);
    while (!q.empty()) {
      P p = q.top();
      q.pop();
      int v = p.second;
      if (dist[v] < p.first)
        continue;
      for (int i = 0; i < (int) g[v].size(); i++) {
        _edge& e = g[v][i];
        UCost c = dist[v] + e.cost + dual[v] - dual[e.to];
        if (e.cap > 0 and dist[e.to] > c) {
          dist[e.to] = c, prevv[e.to] = v, preve[e.to] = i;
          assert(dist[e.to] >= 0);
          q.emplace(dist[e.to], e.to);
        }
      }
    }
  }

  MinCostFlow(int V = 0) : _is_dual_infeasible(false), V(V), g(V), dual(V, 0) {
    static_assert(INF_COST > 0, "INF_COST must be positive");
  }

  int add_arc(int from, int to, Cap cap, Cost cost) {
    assert(0 <= from and from < V);
    assert(0 <= to and to < V);
    assert(cap >= 0);
    if (cost < 0)
      _is_dual_infeasible = true;
    pos.emplace_back(from, g[from].size());
    g[from].push_back({to, (int) g[to].size() + (from == to), cap, cost});
    g[to].push_back({from, (int) g[from].size() - 1, (Cap) 0, -cost});
    return int(pos.size()) - 1;
  }

  // Flush flow f from s to t. Graph must not have negative cycle.
  std::pair<Cap, Cost> flow(int s, int t, const Cap& flow_limit) {
    if (!initialize_dual())
      throw;// Fail to find feasible dual
    Cost cost = 0;
    Cap flow_rem = flow_limit;
    while (flow_rem > 0) {
      _dijkstra(s);
      if (dist[t] == INF_COST)
        break;
      for (int v = 0; v < V; v++)
        dual[v] = std::min(dual[v] + dist[v], INF_COST);
      Cap d = flow_rem;
      for (int v = t; v != s; v = prevv[v])
        d = std::min(d, g[prevv[v]][preve[v]].cap);
      flow_rem -= d;
      cost += d * (dual[t] - dual[s]);
      for (int v = t; v != s; v = prevv[v]) {
        _edge& e = g[prevv[v]][preve[v]];
        e.cap -= d;
        g[v][e.rev].cap += d;
      }
    }
    return std::make_pair(flow_limit - flow_rem, cost);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MCF_HPP_
