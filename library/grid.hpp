//
// Created by zjsdu on 5/30/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
#include <vector>
#include <cassert>
class grid {
  int n, m;

 public:
  grid(int row, int col) {
    assert(row > 0 && col > 0);
    n = row;
    m = col;
  }
  inline bool cover(int i, int j) const {
    return i >= 0 and i < n and j >= 0 and j < m;
  }
  static constexpr int dx[] = {0, 0, -1, 1}, dy[] = {-1, 1, 0, 0};
  template<typename Predicate = bool (*)(int, int)>
  inline std::vector<std::pair<int, int>> neighbors(
      int x, int y, Predicate p = [](int x, int y) { return true; }) const {
    assert(cover(x, y));
    std::vector<std::pair<int, int>> res;
    for (int i = 0; i < 4; i++) {
      if (cover(x + dx[i], y + dy[i]) and p(x + dx[i], y + dy[i]))
        res.emplace_back(x + dx[i], y + dy[i]);
    }
    return res;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
