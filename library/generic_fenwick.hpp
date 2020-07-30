// Created by zjsdu on 7/30/2020.
#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GENERIC_FENWICK_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GENERIC_FENWICK_HPP_
#include <functional>
template<typename T> class fenwick {
  using BinOp = std::function<T(T, T)>;
  BinOp bin_op;
  const T init;
  // 0-indexed
  std::vector<T> a;

 public:
  explicit fenwick(
      int n, BinOp op = [](T x, T y) { return x + y; }, T init = T{})
      : bin_op(std::move(op)), init(init), a(n, init) {}
  T prefix(int x) const {
    auto res = init;
    while (x >= 0) {
      res = bin_op(a[x], res);
      x = (x & (x + 1)) - 1;
    }
    return res;
  }
  void modify(int x, T v) {
    while (x < SZ(a)) {
      a[x] = bin_op(a[x], v);
      x |= (x + 1);
    }
  }
  void clear() { fill(a.begin(), a.end(), init); }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GENERIC_FENWICK_HPP_
