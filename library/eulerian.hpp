//
// Created by zjsdu on 6/6/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TASKS_EULERIAN_HPP_
#define JHELPER_EXAMPLE_PROJECT_TASKS_EULERIAN_HPP_
#include <optional>
#include <undigraph.hpp>
/// \brief Try to find an Eulerian path or cycle starting from a specified
/// vertex u on an UNDIRECTED graph g.
/// \return A list vertices of the found Eulerian path or cycle from vertex u.
std::optional<std::vector<int>> find_eulerian_path_from(int u,
                                                        const undigraph &g) {
  std::vector<int> res(g.e_size_enabled() + 1);
  int stack_ptr = 0;
  int write_ptr = g.e_size_enabled();
  std::vector<bool> used(g.e_size());
  // Non-recursive implementation of dfs.
  // Each search-after-backtrace finds a cycle.
  std::vector<int> ptr(g.v_size());
  res[0] = u;
  while (true) {
    bool found = false;
    while (ptr[u] < (int) g.edges_at(u).size()) {
      int id = g.edges_at(u)[ptr[u]++];
      if (not g.is_enabled(id) or used[id])
        continue;
      used[id] = true;
      auto &e = g.edge(id);
      u ^= e.u ^ e.v;
      res[++stack_ptr] = u;
      found = true;
      break;
    }
    if (not found) {
      res[write_ptr--] = u;
      if (stack_ptr == 0)
        break;
      u = res[--stack_ptr];
    }
  }
  if (write_ptr != -1)
    return nullopt;
  return res;
}
/// \brief Try to find an Eulerian path or cycle on an UNDIRECTED graph g.
/// \return A list vertices of the found Eulerian path or cycle.
std::optional<std::vector<int>> find_eulerian_path(const undigraph &g) {
  int n = g.v_size();
  int odd = 0;
  int first_odd = -1;
  for (int i = 0; i < n; ++i) {
    if (g.degree(i) & 1) {
      if (first_odd == -1)
        first_odd = i;
      odd++;
    }
  }
  if (odd > 2)
    return nullopt;
  return find_eulerian_path_from(first_odd == -1 ? 0 : first_odd, g);
}
#endif// JHELPER_EXAMPLE_PROJECT_TASKS_EULERIAN_HPP_
