//
// Created by zjsdu on 2022/11/9.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SOLVE_LINEAR_SYSTEM_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SOLVE_LINEAR_SYSTEM_HPP_

#include <vector>
#include <cassert>
#include <gauss_jordan.hpp>

// Warning：this function is not tested.
template<typename F>
std::vector<F> solve_linear_system(std::vector<std::vector<F>> a,
                                   const std::vector<F>& b) {
  int h = (int) a.size();
  assert(h > 0);
  assert((int) b.size() == h);
  int w = (int) a[0].size();
  for (int i = 0; i < h; i++)
    assert(w == (int) a[i].size());
  for (int i = 0; i < h; i++)
    a[i].push_back(b[i]);
  std::vector<int> leading_col = gauss_jordan(a, w);
  int rank = (int) leading_col.size();
  for (int i = rank; i < h; i++)
    if (a[i][w])
      return {};
  std::vector<F> sol(w);
  for (int i = 0; i < rank; i++)
    sol[leading_col[i]] = a[i][w];
  return sol;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SOLVE_LINEAR_SYSTEM_HPP_
