//
// Created by zjsdu on 5/30/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
#include <utility>
#include <cassert>
class grid {
  const int n, m;
  static constexpr int dx[] = {0, 0, -1, 1}, dy[] = {-1, 1, 0, 0};

 public:
  grid(int height, int width) : n(height), m(width) { assert(n > 0 && m > 0); }
  struct cell_neighbors;
  struct neighbor_iterator {
    int value;
    const cell_neighbors *const center;
    neighbor_iterator(int value, const cell_neighbors *center)
        : value(value), center(center) {}
    bool operator!=(const neighbor_iterator &other) const {
      return value != other.value;
    }
    std::pair<int, int> operator*() const {
      assert(value != 4);
      return {center->r + dx[value], center->c + dy[value]};
    }
    neighbor_iterator &operator++() {
      while (value < 4) {
        ++value;
        int r = center->r + dx[value];
        int c = center->c + dy[value];
        if (center->g->cover(r, c))
          break;
      }
      return *this;
    }
  };
  struct cell_neighbors {
    const grid *const g;
    const int r, c;
    neighbor_iterator begin() const {
      neighbor_iterator it(-1, this);
      return ++it;
    }
    neighbor_iterator end() const { return neighbor_iterator(4, this); }
  };

  inline bool cover(int i, int j) const {
    return 0 <= i and i < n and 0 <= j and j < m;
  }

  inline cell_neighbors neighbors(int r, int c) const {
    assert(cover(r, c));
    return cell_neighbors{this, r, c};
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRID_HPP_
