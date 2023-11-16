// Created by zjsdu on 7/30/2020.
#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GENERIC_FENWICK_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GENERIC_FENWICK_HPP_
#include <vector>
#include <algorithm>
template<typename T, typename BinOp = const T &(*) (const T &, const T &)>
class fenwick {
  BinOp bin_op;
  const T identity;
  // 0-indexed
  std::vector<T> a;

 public:
  explicit fenwick(int n, BinOp op, T identity)
      : bin_op(op), identity(identity), a(n, identity) {}
  T prefix(int x) const {
    auto res = identity;
    while (x >= 0) {
      res = bin_op(a[x], res);
      x = (x & (x + 1)) - 1;
    }
    return res;
  }
  void modify(int x, T v) {
    while (x < (int) a.size()) {
      a[x] = bin_op(a[x], v);
      x |= (x + 1);
    }
  }
  void clear() { std::fill(a.begin(), a.end(), identity); }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GENERIC_FENWICK_HPP_
