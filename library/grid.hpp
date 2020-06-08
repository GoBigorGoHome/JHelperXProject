//
// Created by zjsdu on 5/30/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
template<typename T> class grid {
  int n, m;
  T *table = nullptr;

 public:
  using value_type = typename T::value_type::value_type;

 public:
  grid(int row, int col) {
    assert(row > 0 && col > 0);
    n = row;
    m = col;
  }
  grid(int row, int col, T &t) : table(&t) {
    assert(row > 0 && col > 0);
    n = row;
    m = col;
  }
  inline bool cover(int i, int j) const {
    return i >= 0 and i < n && j >= 0 && j < m;
  }
  static constexpr int dx[] = {0, 0, -1, 1}, dy[] = {-1, 1, 0, 0};
  inline std::vector<std::pair<int, int>> rc(int x, int y) const {
    assert(cover(x, y));
    std::vector<std::pair<int, int>> res;
    for (int i = 0; i < 4; i++) {
      if (cover(x + dx[i], y + dy[i]))
        res.emplace_back(x + dx[i], y + dy[i]);
    }
    return res;
  }
  inline std::vector<value_type *> neighbor(int x, int y) const {
    assert(cover(x, y));
    std::vector<value_type*> res;
    for (int i = 0; i < 4; i++) {
      if (cover(x + dx[i], y + dy[i]))
        res.push_back(&((*table)[x + dx[i]][y + dy[i]]));
    }
    return res;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
