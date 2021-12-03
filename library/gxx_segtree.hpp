//
// Created by zjsdu on 11/5/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GXX_SEGTREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GXX_SEGTREE_HPP_
#include <functional>
#include <vector>
#include <cassert>
//线段树是满二叉树。一棵有n个叶子的满二叉树共有2*n-1个节点。
//节点编号方式：index(l, r) := (l + r) | (l != r)。

template<typename Value, typename Tag, typename Result> class SegTree {
  int index(int l, int r) const { return (l + r) | (l != r); }
  struct Node {
    int p = -1;
    Value val;
    Tag tag;
    void apply(int l, int r, const Tag &t) {
      tag.apply(t);
      val.apply(l, r, t);
    }
  };
  int n;
  std::vector<Node> tree;
  inline void push(int l, int r) {
    auto &t = tree[index(l, r)].tag;
    if (t.pushable()) {
      int m = (l + r) >> 1, y = index(l, m), z = index(m + 1, r);
      tree[y].val.apply(l, m, t);
      tree[y].tag.apply(t);
      tree[z].val.apply(m + 1, r, t);
      tree[z].tag.apply(t);
      t.after_push();
    }
  }
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
  Result get(int l, int r, int ll, int rr) {
    if (ll <= l && r <= rr)
      return tree[index(l, r)].val;
    int m = (l + r) >> 1;
    push(l, r);
    if (rr <= m)
      return get(l, m, ll, rr);
    if (ll > m)
      return get(m + 1, r, ll, rr);
    return get(l, m, ll, rr) + get(m + 1, r, ll, rr);
  }
  void modify(int l, int r, int ll, int rr, const Tag &t) {
    if (ll <= l && r <= rr) {
      tree[index(l, r)].apply(l, r, t);
      return;
    }
    push(l, r);
    int m = (l + r) >> 1;
    if (ll <= m)
      modify(l, m, ll, rr, t);
    if (rr > m)
      modify(m + 1, r, ll, rr, t);
    pull(l, r);
  }
  int find_first(int l, int r, int ll, int rr,
                 const std::function<bool(const Value &)> &f) {
    if (ll <= l && r <= rr)
      return f(tree[index(l, r)].val) ? find_first_knowingly(l, r, f) : -1;
    push(l, r);
    int m = (l + r) >> 1;
    int res = -1;
    if (ll <= m)
      res = find_first(l, m, ll, rr, f);
    if (rr > m && res == -1)
      res = find_first(m + 1, r, ll, rr, f);
    return res;
  }
  int find_last(int l, int r, int ll, int rr,
                const std::function<bool(const Value &)> &f) {
    if (ll <= l && r <= rr)
      return f(tree[index(l, r)].val) ? find_last_knowingly(l, r, f) : -1;
    push(l, r);
    int m = (l + r) >> 1;
    int res = -1;
    if (rr > m)
      res = find_last(m + 1, r, ll, rr, f);
    if (ll <= m && res == -1)
      res = find_last(l, m, ll, rr, f);
    return res;
  }
  int find_first_knowingly(int l, int r,
                           const std::function<bool(const Value &)> &f) {
    if (l == r)
      return l;
    push(l, r);
    int m = (l + r) >> 1;
    return f(tree[index(l, m)].val) ? find_first_knowingly(l, m, f)
                                    : find_first_knowingly(m + 1, r, f);
  }
  int find_last_knowingly(int l, int r,
                          const std::function<bool(const Value &)> &f) {
    if (l == r)
      return l;
    push(l, r);
    int m = (l + r) >> 1;
    return f(tree[index(m + 1, r)].val) ? find_last_knowingly(m + 1, r, f)
                                        : find_last_knowingly(l, m, f);
  }

 public:
  template<typename F = void (*)(Value &, int)>
  SegTree(
      int _n, const F &f = [](Value &, int) {})
      : n(_n) {
    assert(n > 0);
    tree.resize(2 * n - 1);
    build(0, n - 1, f);
  }
  Result get() { return tree[index(0, n - 1)].val; }
  Result get(int ll, int rr) {
    if (ll > rr)
      return Result{};
    assert(0 <= ll && rr <= n - 1);
    return get(0, n - 1, ll, rr);
  }
  Result get(int p) {
    assert(0 <= p && p <= n - 1);
    return get(0, n - 1, p, p);
  }
  void modify(int ll, int rr, const Tag &t) {
    if (ll > rr)
      return;
    assert(0 <= ll && rr <= n - 1);
    modify(0, n - 1, ll, rr, t);
  }
  template<typename F> void modify(int leaf, const F &f) {
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
  Tag collect_tags(int leaf) const {
    Tag t;
    assert(0 <= leaf && leaf < n);
    for (int i = 2 * leaf; i != -1; i = tree[i].p | 1)
      t.apply(tree[i].tag);
    return t;
  }
  // find_first and find_last call all FALSE elements
  // to the left (right) of the sought position exactly once
  int find_first(int ll, int rr, const std::function<bool(const Value &)> &f) {
    if (ll > rr)
      return -1;
    assert(0 <= ll && rr <= n - 1);
    return find_first(0, n - 1, ll, rr, f);
  }
  int find_last(int ll, int rr, const std::function<bool(const Value &)> &f) {
    if (ll > rr)
      return -1;
    assert(0 <= ll && rr <= n - 1);
    return find_last(0, n - 1, ll, rr, f);
  }
};
template<typename Result>
using segtree_t = SegTree<struct Value, struct Tag, Result>;
using segtree = segtree_t<struct Value>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GXX_SEGTREE_HPP_
