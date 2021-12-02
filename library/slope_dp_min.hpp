//
// Created by zjsdu on 2021/12/1.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SLOPE_DP_MIN_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SLOPE_DP_MIN_HPP_
#include <rational.hpp>
#include <cassert>
#include <functional>

/// @brief slope_dp_min solves 1D/1D DP f(x) = min_{y \< x} f(y) + cost(y, x)
/// where cost(y, x) has the form a(x) + b(y) - c(x) * d(y) (the constant item
/// is included in a(x)) and the function d(y) is strictly increasing. The
/// technique is known as the convex hull trick.
template<typename DomainType, typename ValueType> class slope_dp_min {
  using point = std::pair<ValueType, ValueType>;
  using rat = rational<ValueType>;

  std::vector<point> q;
  std::function<ValueType(DomainType)> a, b, c, d;

  ValueType get_value(int i, DomainType x) {
    return a(x) + q[i].first - q[i].second * c(x);
  }

  rat slope(const point& p1, const point& p2) {
    return rat(p2.first - p1.first, p2.second - p1.second);
  }

 public:
  template<typename FuncA, typename FuncB, typename FuncC, typename FuncD>
  /// @pre The function \p d shall be strictly increasing.
  slope_dp_min(FuncA a, FuncB b, FuncC c, FuncD d) : a(a), b(b), c(c), d(d){};

  void init(DomainType x, ValueType value) {
    q.emplace_back(value + b(x), d(x));
  }

  ValueType get(DomainType x) {
    assert(not q.empty());
    auto C = c(x);
    int l = 0, r = (int) q.size() - 2;
    while (l <= r) {
      int mid = (l + r) / 2;
      if (slope(q[mid], q[mid + 1]) > C) {
        r = mid - 1;
      } else {
        l = mid + 1;
      }
    }
    ValueType v = get_value(l, x);
    point cur(v + b(x), d(x));
    while (q.size() >= 2) {
      auto p1 = q.rbegin(), p2 = p1 + 1;
      // TODO: prove that condition `slope(cur, *p1) > slope(*p1, *p2)` is
      // equivalent to `slope(cur, *p1) > slope(cur, *p2)`.
      if (slope(cur, *p1) > slope(*p1, *p2))
        break;
      q.pop_back();
    }
    q.push_back(cur);
    return v;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SLOPE_DP_MIN_HPP_
