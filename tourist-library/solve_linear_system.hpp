//
// Created by zjsdu on 2022/11/8.
//


#ifndef JHELPER_EXAMPLE_PROJECT_TOURIST_LIBRARY_SOLVE_LINEAR_SYSTEM_HPP_
#define JHELPER_EXAMPLE_PROJECT_TOURIST_LIBRARY_SOLVE_LINEAR_SYSTEM_HPP_

#include <vector>
#include <cassert>

template<typename T>
std::vector<T> SolveLinearSystem(std::vector<std::vector<T>> a,
                                 const std::vector<T>& b, int w) {
  int h = static_cast<int>(a.size());
  assert(h == static_cast<int>(b.size()));
  if (h > 0) {
    assert(w == static_cast<int>(a[0].size()));
  }
  for (int i = 0; i < h; i++) {
    a[i].push_back(b[i]);
  }
  GaussianElimination(a, w);
  std::vector<T> x(w, 0);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (!IsZero(a[i][j])) {
        x[j] = a[i][w] / a[i][j];
        break;
      }
    }
  }
  return x;
}
#endif// JHELPER_EXAMPLE_PROJECT_TOURIST_LIBRARY_SOLVE_LINEAR_SYSTEM_HPP_
