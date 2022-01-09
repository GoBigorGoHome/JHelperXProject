//
// Created by zjsdu on 5/28/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_OLD_SEGTREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_OLD_SEGTREE_HPP_
#include <functional>
#include <vector>
#include <cassert>
//线段树是满二叉树。一棵有n个叶子的满二叉树共有2*n-1个节点。
//按先序遍历的顺序给节点编号。

template<typename Value, typename Tag, typename Result> class SegTree {
  struct Node {
    Value val;
    Tag tag;
    template<typename... M> void apply(int l, int r, const M&... v) {
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
  inline void push(int x, int l, int r) {
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    // parent: x, left son: x + 1, right son: z
    if (tree[x].tag) {
      tree[x + 1].val.apply(tree[x].tag, l, y);
      tree[x + 1].tag += tree[x].tag;
      tree[z].val.apply(tree[x].tag, y + 1, r);
      tree[z].tag += tree[x].tag;
      tree[x].tag = empty_tag;
    }
  }
  inline void pull(int x, int z) {
    tree[x].val = tree[x + 1].val + tree[z].val;
  }
  void build(int x, int l, int r) {
    if (l == r) {
      tree[x].val.init_leaf(l);
      return;
    }
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    build(x + 1, l, y);
    build(z, y + 1, r);
    pull(x, z);
  }
  Result get(int x, int l, int r, int ll, int rr) {
    if (ll <= l && r <= rr) {
      return tree[x].val;
    }
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    push(x, l, r);
    if (rr <= y)
      return get(x + 1, l, y, ll, rr);
    if (ll > y)
      return get(z, y + 1, r, ll, rr);
    return get(x + 1, l, y, ll, rr) + get(z, y + 1, r, ll, rr);
  }
  template<typename... M>
  void modify_leaf(int x, int l, int r, int p, const M&... v) {
    if (l == r) {
      tree[x].val.apply_to_leaf(v...);
      return;
    }
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    push(x, l, r);
    if (p <= y)
      modify_leaf(x + 1, l, y, p, v...);
    else
      modify_leaf(z, y + 1, r, p, v...);
    pull(x, z);
  }
  template<typename... M>
  void modify(int x, int l, int r, int ll, int rr, const M&... v) {
    if (ll <= l && r <= rr) {
      tree[x].apply(l, r, v...);
      return;
    }
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    push(x, l, r);
    if (ll <= y) {
      modify(x + 1, l, y, ll, rr, v...);
    }
    if (rr > y) {
      modify(z, y + 1, r, ll, rr, v...);
    }
    pull(x, z);
  }
  int find_first(int x, int l, int r, int ll, int rr,
                 const std::function<bool(const Value&)>& f) {
    if (ll <= l && r <= rr) {
      if (!f(tree[x].val)) {
        return -1;
      }
      return find_first_knowingly(x, l, r, f);
    }
    push(x, l, r);
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    int res = -1;
    if (ll <= y) {
      res = find_first(x + 1, l, y, ll, rr, f);
    }
    if (rr > y && res == -1) {
      res = find_first(z, y + 1, r, ll, rr, f);
    }
    return res;
  }
  int find_last(int x, int l, int r, int ll, int rr,
                const std::function<bool(const Value&)>& f) {
    if (ll <= l && r <= rr) {
      if (!f(tree[x].val)) {
        return -1;
      }
      return find_last_knowingly(x, l, r, f);
    }
    push(x, l, r);
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    int res = -1;
    if (rr > y) {
      res = find_last(z, y + 1, r, ll, rr, f);
    }
    if (ll <= y && res == -1) {
      res = find_last(x + 1, l, y, ll, rr, f);
    }
    return res;
  }
  int find_first_knowingly(int x, int l, int r,
                           const std::function<bool(const Value&)>& f) {
    if (l == r) {
      return l;
    }
    push(x, l, r);
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    int res;
    if (f(tree[x + 1].val)) {
      res = find_first_knowingly(x + 1, l, y, f);
    } else {
      res = find_first_knowingly(z, y + 1, r, f);
    }
    return res;
  }
  int find_last_knowingly(int x, int l, int r,
                          const std::function<bool(const Value&)>& f) {
    if (l == r) {
      return l;
    }
    push(x, l, r);
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    int res;
    if (f(tree[z].val)) {
      res = find_last_knowingly(z, y + 1, r, f);
    } else {
      res = find_last_knowingly(x + 1, l, y, f);
    }
    return res;
  }
  template<typename... Args>
  void collect(int x, int l, int r, int p, Args&&... args) const {
    tree[x].tag.collect(std::forward<Args...>(args...));
    if (l == r)
      return;
    int y = (l + r) >> 1;
    int z = x + ((y - l + 1) << 1);
    if (p <= y)
      collect(x + 1, l, y, p, std::forward<Args...>(args...));
    else
      collect(z, y + 1, r, p, std::forward<Args...>(args...));
  }

 public:
  SegTree(int _n) : n(_n) {
    assert(n > 0);
    tree.resize(2 * n - 1);
    build(0, 0, n - 1);
  }
  Result get() { return tree[0].val; }
  Result get(int ll, int rr) {
    assert(0 <= ll && ll <= rr && rr <= n - 1);
    return get(0, 0, n - 1, ll, rr);
  }
  Result get(int p) {
    assert(0 <= p && p <= n - 1);
    return get(0, 0, n - 1, p, p);
  }
  template<typename... M> void modify(int ll, int rr, const M&... v) {
    if (ll > rr)
      return;
    assert(0 <= ll && rr <= n - 1);
    modify(0, 0, n - 1, ll, rr, v...);
  }
  template<typename... M> void modify_leaf(int p, const M&... v) {
    assert(0 <= p && p < n);
    modify_leaf(0, 0, n - 1, p, v...);
  }
  template<typename... Args> void collect(int p, Args&&... args) const {
    collect(0, 0, n - 1, p, std::forward<Args...>(args...));
  }
  // find_first and find_last call all FALSE elements
  // to the left (right) of the sought position exactly once
  int find_first(int ll, int rr, const std::function<bool(const Value&)>& f) {
    assert(0 <= ll && ll <= rr && rr <= n - 1);
    return find_first(0, 0, n - 1, ll, rr, f);
  }
  int find_last(int ll, int rr, const std::function<bool(const Value&)>& f) {
    assert(0 <= ll && ll <= rr && rr <= n - 1);
    return find_last(0, 0, n - 1, ll, rr, f);
  }
};
template<typename Result>
using segtree_t = SegTree<struct Value, struct Tag, Result>;
using segtree = segtree_t<struct Value>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_OLD_SEGTREE_HPP_
