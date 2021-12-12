//
// Created by zjs on 2021/12/12.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STATIC_2D_RANGE_SUM_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STATIC_2D_RANGE_SUM_HPP_
#include <vector>
#include <cassert>
#include <algorithm>
#include <tuple>

/// @tparam CoordinateType must be an integral type.
template<typename CoordinateType, typename ValueType>
class static_2d_range_sum {
  int n = 0;
  bool ready = false;
  using data_type = std::tuple<CoordinateType, CoordinateType, ValueType>;
  std::vector<data_type> points;
  using YV = std::pair<CoordinateType, ValueType>;
  std::vector<std::vector<YV>> cumulative_sum;

  ValueType inner_sum(const std::vector<YV>& vec, CoordinateType y) const {
    auto iter = std::upper_bound(
        vec.begin(), vec.end(), y,
        [](int y, const YV& element) { return y < element.first; });
    return iter == vec.begin() ? ValueType(0) : (iter - 1)->second;
  }

 public:
  void init() {
    assert(not ready);
    std::sort(points.begin(), points.end(),
              [](const data_type& a, const data_type& b) {
                return std::get<0>(a) < std::get<0>(b);
              });
    cumulative_sum.resize(n);
    for (int i = 0; i < n; i++) {
      auto [x, y, v] = points[i];
      for (int j = i; j < n; j |= j + 1) {
        cumulative_sum[j].emplace_back(y, v);
      }
    }
    for (int i = 0; i < n; i++) {
      std::sort(cumulative_sum[i].begin(), cumulative_sum[i].end(),
                [](const YV& a, const YV& b) { return a.first < b.first; });
      ValueType sum = 0;
      for (auto& p : cumulative_sum[i]) {
        sum += p.second;
        p.second = sum;
      }
    }
    ready = true;
  }

  void add(CoordinateType x, CoordinateType y, ValueType value) {
    assert(not ready);
    points.emplace_back(x, y, value);
    n += 1;
  }

  ValueType sum(CoordinateType x, CoordinateType y) const {
    assert(ready);
    auto iter = std::upper_bound(points.begin(), points.end(), x,
                                 [](CoordinateType x, const data_type& data) {
                                   return x < std::get<0>(data);
                                 });
    ValueType res = 0;
    for (auto i = iter - points.begin() - 1; i >= 0; i = (i & (i + 1)) - 1) {
      res += inner_sum(cumulative_sum[i], y);
    }
    return res;
  }

  ValueType sum(CoordinateType x1, CoordinateType x2, CoordinateType y1,
                CoordinateType y2) const {
    assert(ready);
    assert(x1 <= x2 and y1 <= y2);
    return sum(x2, y2) - sum(x2, y1 - 1) - sum(x1 - 1, y2)
        + sum(x1 - 1, y1 - 1);
  }
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STATIC_2D_RANGE_SUM_HPP_
