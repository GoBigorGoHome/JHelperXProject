//
// Created by zjsdu on 8/1/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_ROOTED_FOREST_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_ROOTED_FOREST_HPP_
#include <vector>
#include <cassert>
// A simple class that builds a sparse ancestor table for a rooted forest.
// It can answer a query of the following two types in logarithmic time:
// 1. the highest ancestor of a vertex that meet some conditions, and
// 2. the k-th ancestor of a vertex.
class rooted_forest {
 public:
  explicit rooted_forest(const std::vector<int> &parent) {
    n = (int) parent.size();
    log = 0;
    int t = 1;
    while (t * 2 < n) {
      t *= 2;
      ++log;
    }
    anc.assign(log + 1, std::vector<int>(n, -1));
    anc[0] = parent;
    for (int i = 0; i < log; i++)
      for (int j = 0; j < n; j++)
        if (anc[i][j] != -1)
          anc[i + 1][j] = anc[i][anc[i][j]];
  }

  /// \brief  Binary search for the top ancestor of vertex v for which
  /// predicate p returns true.
  /// \param v The vertex where the search begins.
  /// \param p Unary predicate which returns true for vertices from v up to some
  /// ancestor of v.
  /// \return The top ancestor of v for which p returns true.
  template<typename UnaryPredicate>
  int top_ancestor(int v, UnaryPredicate p) const {
    assert(0 <= v and v < n);
    assert(p(v));
    for (int i = log; i >= 0; i--) {
      if (anc[i][v] != -1 and p(anc[i][v]))
        v = anc[i][v];
    }
    return v;
  }

  /// \return the d-th ancestor of vertex u.
  int ancestor(int u, int d) const {
    assert(0 <= u and u < n);
    for (int i = 0; i <= log; i++)
      if ((d >> i) & 1) {
        u = anc[i][u];
        if (u == -1)
          break;
      }
    return u;
  }

 private:
  int log;
  int n;
  std::vector<std::vector<int>> anc;
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_ROOTED_FOREST_HPP_
