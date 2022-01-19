//
// Created by zjsdu on 2022/1/19.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_ALGO_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_ALGO_HPP_

#include <vector>
#include <cassert>
#include <cmath>
#include <numeric>
#include <algorithm>

/// @tparam T Type of answer.
template<typename T, T (*dec_x)(T, int, int), T (*inc_x)(T, int, int),
         T (*dec_y)(T, int, int), T (*inc_y)(T, int, int)>
class mo_algo {
  using Point = std::pair<int, int>;
  std::vector<Point> points_;
  std::vector<int> id;

 public:
  explicit mo_algo(const std::vector<std::pair<int, int>>& points)
      : points_(points), id(points.size()) {

    assert(not points.empty());
    int min_x = points_[0].first;
    int max_x = min_x;

    for (auto p : points_) {
      min_x = std::min(min_x, p.first);
      max_x = std::max(max_x, p.first);
    }
    const int block_len = std::max(1, (int) std::sqrt(max_x - min_x));

    int n = (int) points_.size();
    std::vector<int> block(n);
    for (int i = 0; i < n; i++)
      block[i] = (points_[i].first - min_x) / block_len;

    std::iota(id.begin(), id.end(), 0);
    std::sort(id.begin(), id.end(), [&](int i, int j) {
      if (block[i] != block[j])
        return block[i] < block[j];
      return points_[i].second < points_[j].second;
    });
  }

  std::vector<T> calc(int x0, int y0, T v0) const {
    auto work = [&](int x, int y) {
      while (x0 < x) {
        v0 = inc_x(v0, x0, y0);
        x0++;
      }
      while (x0 > x) {
        v0 = dec_x(v0, x0, y0);
        x0--;
      }
      while (y0 < y) {
        v0 = inc_y(v0, x0, y0);
        y0++;
      }
      while (y0 > y) {
        v0 = dec_y(v0, x0, y0);
        y0--;
      }
    };
    std::vector<T> res(points_.size());
    for (int i : id) {
      work(points_[i].first, points_[i].second);
      res[i] = v0;
    }
    return res;
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MO_ALGO_HPP_
