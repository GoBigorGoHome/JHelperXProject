//
// Created by zjsdu on 2022/11/8.
//
// author: tourist
// source:
#ifndef JHELPER_EXAMPLE_PROJECT_TOURIST_LIBRARY_GAUSSIAN_ELIMINATION_HPP_
#define JHELPER_EXAMPLE_PROJECT_TOURIST_LIBRARY_GAUSSIAN_ELIMINATION_HPP_

#include <vector>
#include <cassert>
#include <cmath>

const double eps = 1e-9;

bool IsZero(double v) {
  return std::abs(v) < 1e-9;
}

enum GAUSS_MODE { DEGREE, ABS };

// 用初等行变换把增广矩阵a化成特殊的行阶梯形。特殊之处：每一行至多有一个非零项。
// 当系数矩阵是方阵时，保持行列式不变（相似标准形？）。
// limit：未知数个数（即系数矩阵的列数）。
// mode：选主元的方式。DEGREE：有限域，非零项最少。ABS：实数域，绝对值最大。
template<typename T>
void GaussianElimination(std::vector<std::vector<T>>& a, int limit,
                         GAUSS_MODE mode = DEGREE) {
  if (a.empty() || a[0].empty()) {
    return;
  }
  int h = static_cast<int>(a.size());
  int w = static_cast<int>(a[0].size());
  for (int i = 0; i < h; i++) {
    assert(w == static_cast<int>(a[i].size()));
  }
  assert(limit <= w);
  std::vector<int> deg(h);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      deg[i] += !IsZero(a[i][j]);
    }
  }
  int r = 0;
  for (int c = 0; c < limit; c++) {
    int id = -1;
    for (int i = r; i < h; i++) {
      if (!IsZero(a[i][c])
          && (id == -1 || (mode == DEGREE && deg[i] < deg[id])
              || (mode == ABS && abs(a[id][c]) < abs(a[i][c])))) {
        id = i;
      }
    }
    if (id == -1) {
      continue;
    }
    if (id > r) {
      std::swap(a[r], a[id]);
      std::swap(deg[r], deg[id]);
      for (int j = c; j < w; j++) {
        a[id][j] = -a[id][j];
      }
    }
    std::vector<int> nonzero;
    for (int j = c; j < w; j++) {
      if (!IsZero(a[r][j])) {
        nonzero.push_back(j);
      }
    }
    T inv_a = 1 / a[r][c];
    for (int i = r + 1; i < h; i++) {
      if (IsZero(a[i][c])) {
        continue;
      }
      T coeff = -a[i][c] * inv_a;
      for (int j : nonzero) {
        if (!IsZero(a[i][j]))
          deg[i]--;
        a[i][j] += coeff * a[r][j];
        if (!IsZero(a[i][j]))
          deg[i]++;
      }
    }
    ++r;
  }
  for (r = h - 1; r >= 0; r--) {
    for (int c = 0; c < limit; c++) {
      if (!IsZero(a[r][c])) {
        T inv_a = 1 / a[r][c];
        for (int i = r - 1; i >= 0; i--) {
          if (IsZero(a[i][c])) {
            continue;
          }
          T coeff = -a[i][c] * inv_a;
          for (int j = c; j < w; j++) {
            a[i][j] += coeff * a[r][j];
          }
        }
        break;
      }
    }
  }
}
#endif// JHELPER_EXAMPLE_PROJECT_TOURIST_LIBRARY_GAUSSIAN_ELIMINATION_HPP_
