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
    template<typename... M> void apply(int l, int r, const M &... v) {
      if constexpr (Tag::additive) {
        Tag delta{v...};
        tag += delta;
        val.apply(delta, l, r);
      } else {
        tag.apply(v...);
      }
    }
  };
  const Tag empty_tag{};
  int n;
  std::vector<Node> tree;
  inline void push(int l, int r) {
    int x = index(l, r), m = (l + r) >> 1, y = index(l, m), z = index(m + 1, r);
    if (tree[x].tag) {
      tree[y].val.apply(tree[x].tag, l, m);
      tree[y].tag += tree[x].tag;
      tree[z].val.apply(tree[x].tag, m + 1, r);
      tree[z].tag += tree[x].tag;
      tree[x].tag = empty_tag;
    }
  }
  inline void pull(int l, int r) {
    int x = index(l, r), m = (l + r) >> 1, y = index(l, m), z = index(m + 1, r);
    tree[x].val = tree[y].val + tree[z].val;
  }
  void build(int l, int r) {
    if (l == r) {
      tree[2 * l].val.init_leaf(l);
      return;
    }
    int m = (l + r) >> 1;
    tree[index(l, m)].p = tree[index(m + 1, r)].p = l + r;
    build(l, m);
    build(m + 1, r);
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
  template<typename... M>
  void modify(int l, int r, int ll, int rr, const M &... v) {
    if (ll <= l && r <= rr) {
      tree[index(l, r)].apply(l, r, v...);
      return;
    }
    push(l, r);
    int m = (l + r) >> 1;
    if (ll <= m)
      modify(l, m, ll, rr, v...);
    if (rr > m)
      modify(m + 1, r, ll, rr, v...);
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
  SegTree(int _n) : n(_n) {
    assert(n > 0);
    tree.resize(2 * n - 1);
    build(0, n - 1);
  }
  Result get() { return tree[0].val; }
  Result get(int ll, int rr) {
    assert(0 <= ll && ll <= rr && rr <= n - 1);
    return get(0, n - 1, ll, rr);
  }
  Result get(int p) {
    assert(0 <= p && p <= n - 1);
    return get(0, n - 1, p, p);
  }
  template<typename... M> void modify(int ll, int rr, const M &... v) {
    if (ll > rr)
      return;
    assert(0 <= ll && rr <= n - 1);
    modify(0, n - 1, ll, rr, v...);
  }
  template<typename... M> void modify_leaf(int leaf, const M &... v) {
    assert(0 <= leaf && leaf < n);
    tree[2 * leaf].val.apply_to_leaf(v...);
    for (int l = leaf, r = leaf, s; (s = tree[index(l, r)].p) != -1;)
      if (s < l + r) {
        tree[s | 1] = tree[index(s - r, l - 1)] + tree[index(l, r)];
        l = s - r;
      } else {
        tree[s | 1] = tree[index(l, r)] + tree[index(r + 1, s - l)];
        r = s - l;
      }
  }
  template<typename... Args> void collect(int leaf, Args &&... args) const {
    assert(0 <= leaf && leaf < n);
    for (int i = 2 * leaf; i != -1; i = tree[i].p | 1)
      tree[i].tag.collect(std::forward<Args...>(args...));
  }
  // find_first and find_last call all FALSE elements
  // to the left (right) of the sought position exactly once
  int find_first(int ll, int rr, const std::function<bool(const Value &)> &f) {
    assert(0 <= ll && ll <= rr && rr <= n - 1);
    return find_first(0, n - 1, ll, rr, f);
  }
  int find_last(int ll, int rr, const std::function<bool(const Value &)> &f) {
    assert(0 <= ll && ll <= rr && rr <= n - 1);
    return find_last(0, n - 1, ll, rr, f);
  }
};
template<typename Result>
using segtree_t = SegTree<struct Value, struct Tag, Result>;
using segtree = segtree_t<struct Value>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GXX_SEGTREE_HPP_
