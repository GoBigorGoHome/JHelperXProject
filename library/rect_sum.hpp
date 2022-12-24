//
// Created by zjs on 12/17/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_RECT_SUM_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_RECT_SUM_HPP_
#include <vector>
#include <cassert>
// Rows and columns are 1-indexed.
template<typename T> class rect_sum {
  std::vector<std::vector<T>> s;
  const int h, w;

 public:
  rect_sum(int H, int W) : s(H + 1, std::vector<T>(W + 1)), h(H), w(W) {
    assert(h >= 0);
    assert(w >= 0);
  }

  T sum(int x, int y) const {
    assert(x <= h);
    assert(y <= w);
    if (x <= 0 || y <= 0)
      return 0;
    return s[x][y];
  }

  T sum(int x1, int x2, int y1, int y2) const {
    if (x1 < 1)
      x1 = 1;
    if (y1 < 1)
      y1 = 1;
    if (x1 > x2 || y1 > y2)
      return 0;
    assert(x2 <= h);
    assert(y2 <= w);
    return s[x2][y2] - s[x1 - 1][y2] - s[x2][y1 - 1] + s[x1 - 1][y1 - 1];
  }

  void set(int x, int y, T val) {
    if (x < 1 || x > h || y < 1 || y > w)
      return;
    s[x][y] = s[x - 1][y] + s[x][y - 1] - s[x - 1][y - 1] + val;
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_RECT_SUM_HPP_
