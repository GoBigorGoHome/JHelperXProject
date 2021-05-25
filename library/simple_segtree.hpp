//
// Created by zjsdu on 5/25/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SIMPLE_SEGTREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SIMPLE_SEGTREE_HPP_
#include <vector>
#include <cassert>
// A segment tree is a full binary tree.
// Node index scheme：index(l, r) := (l + r) | (l != r).
/// @brief A simple segment tree without tags.
template<typename Value> class simple_segment_tree {
  int index(int l, int r) const { return (l + r) | (l != r); }
  struct Node {
    int p = -1;
    Value val;
  };
  int n;
  std::vector<Node> tree;
  inline void pull(int l, int r) {
    int x = index(l, r), m = (l + r) >> 1, y = index(l, m), z = index(m + 1, r);
    tree[x].val = tree[y].val + tree[z].val;
  }
  template<typename F> void build(int l, int r, const F &f) {
    if (l == r) {
      f(tree[2 * l].val, l);
      return;
    }
    int m = (l + r) >> 1;
    tree[index(l, m)].p = tree[index(m + 1, r)].p = l + r;
    build(l, m, f);
    build(m + 1, r, f);
    pull(l, r);
  }
  Value get(int l, int r, int ll, int rr) {
    if (ll <= l && r <= rr)
      return tree[index(l, r)].val;
    int m = (l + r) >> 1;
    if (rr <= m)
      return get(l, m, ll, rr);
    if (ll > m)
      return get(m + 1, r, ll, rr);
    return get(l, m, ll, rr) + get(m + 1, r, ll, rr);
  }
  template<typename value_predicate>
  int find_first(int l, int r, int ll, int rr, const value_predicate &f) {
    if (ll <= l && r <= rr)
      return f(tree[index(l, r)].val) ? find_first_knowingly(l, r, f) : -1;
    int m = (l + r) >> 1;
    int res = -1;
    if (ll <= m)
      res = find_first(l, m, ll, rr, f);
    if (rr > m && res == -1)
      res = find_first(m + 1, r, ll, rr, f);
    return res;
  }
  template<typename value_predicate>
  int find_last(int l, int r, int ll, int rr, const value_predicate &f) {
    if (ll <= l && r <= rr)
      return f(tree[index(l, r)].val) ? find_last_knowingly(l, r, f) : -1;
    int m = (l + r) >> 1;
    int res = -1;
    if (rr > m)
      res = find_last(m + 1, r, ll, rr, f);
    if (ll <= m && res == -1)
      res = find_last(l, m, ll, rr, f);
    return res;
  }
  template<typename value_predicate>
  int find_first_knowingly(int l, int r, const value_predicate &f) {
    if (l == r)
      return l;
    int m = (l + r) >> 1;
    return f(tree[index(l, m)].val) ? find_first_knowingly(l, m, f)
                                    : find_first_knowingly(m + 1, r, f);
  }
  template<typename value_predicate>
  int find_last_knowingly(int l, int r, const value_predicate &f) {
    if (l == r)
      return l;
    int m = (l + r) >> 1;
    return f(tree[index(m + 1, r)].val) ? find_last_knowingly(m + 1, r, f)
                                        : find_last_knowingly(l, m, f);
  }

 public:
  template<typename init_leaf = void (*)(Value &, int)>
  simple_segment_tree(
      int n, const init_leaf &f = [](Value &, int) {})
      : n(n) {
    assert(n > 0);
    tree.resize(2 * n - 1);
    build(0, n - 1, f);
  }
  Value get() { return tree[index(0, n - 1)].val; }
  Value get(int ll, int rr) {
    if (ll > rr)
      return Value{};
    assert(0 <= ll && rr <= n - 1);
    return get(0, n - 1, ll, rr);
  }
  Value get(int p) {
    assert(0 <= p && p <= n - 1);
    return get(0, n - 1, p, p);
  }
  template<typename value_operation>
  void modify(int leaf, const value_operation &f) {
    assert(0 <= leaf && leaf < n);
    f(tree[2 * leaf].val);
    for (int l = leaf, r = leaf, s; (s = tree[index(l, r)].p) != -1;)
      if (s < l + r) {
        tree[s | 1].val = tree[index(s - r, l - 1)].val + tree[index(l, r)].val;
        l = s - r;
      } else {
        tree[s | 1].val = tree[index(l, r)].val + tree[index(r + 1, s - l)].val;
        r = s - l;
      }
  }
  // find_first and find_last call all FALSE elements
  // to the left (right) of the sought position exactly once
  template<typename value_predicate>
  int find_first(int ll, int rr, const value_predicate &f) {
    if (ll > rr)
      return -1;
    assert(0 <= ll && rr <= n - 1);
    return find_first(0, n - 1, ll, rr, f);
  }
  template <typename value_predicate>
  int find_last(int ll, int rr, const value_predicate &f) {
    if (ll > rr)
      return -1;
    assert(0 <= ll && rr <= n - 1);
    return find_last(0, n - 1, ll, rr, f);
  }
};
using segtree = simple_segment_tree<struct Value>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SIMPLE_SEGTREE_HPP_
