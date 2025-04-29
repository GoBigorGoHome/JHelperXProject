//
// Created by zjs on 4/27/25.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DYNAMIC_LAZY_SEGTREE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DYNAMIC_LAZY_SEGTREE_HPP_
#include <vector>
#include <functional>
#include <cassert>

// @return same with std::bit::bit_ceil
unsigned int bit_ceil(unsigned int n) {
  unsigned int x = 1;
  while (x < (unsigned int) (n))
    x *= 2;
  return x;
}

// @param n `1 <= n`
// @return same with std::bit::countr_zero
int countr_zero(unsigned int n) {
#ifdef _MSC_VER
  unsigned long index;
  _BitScanForward(&index, n);
  return index;
#else
  return __builtin_ctz(n);
#endif
}

#if __cplusplus >= 201703L

template<class S, auto op, auto e, class F, auto mapping, auto composition,
         auto id>
struct dynamic_lazy_segtree {
  static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>,
                "op must work as S(S, S)");
  static_assert(std::is_convertible_v<decltype(e), std::function<S()>>,
                "e must work as S()");
  static_assert(
      std::is_convertible_v<decltype(mapping), std::function<S(F, S)>>,
      "mapping must work as F(F, S)");
  static_assert(
      std::is_convertible_v<decltype(composition), std::function<F(F, F)>>,
      "compostiion must work as F(F, F)");
  static_assert(std::is_convertible_v<decltype(id), std::function<F()>>,
                "id must work as F()");

#else

template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S),
         F (*composition)(F, F), F (*id)()>
struct lazy_segtree {

#endif

  struct node {
    node *p = nullptr;
    node *child[2] = {};
    S v = e();
    F lz = id();
    node(node *p) : p(p) {}
  };

 public:
  explicit dynamic_lazy_segtree(int n) : _n(n) {
    size = (int) bit_ceil((unsigned int) (_n));
    log = countr_zero((unsigned int) size);
    p_root = new node(nullptr);
    root = new node(p_root);
    p_root->child[0] = root;
    p_root->child[1] = root;
  }

  void set(int p, S x) {
    assert(0 <= p && p < _n);
    node *cur = root;
    for (int i = log - 1; i >= 0; i--) {
      push(cur);
      cur = cur->child[p >> i & 1];
    }
    cur->v = x;
    for (int i = 1; i <= log; i++) {
      cur = cur->p;
      update(cur);
    }
  }

  S get(int p) {
    assert(0 <= p && p < _n);
    node *cur = root;
    for (int i = log - 1; i >= 0; i--) {
      push(cur);
      cur = cur->child[p >> i & 1];
    }
    return cur->v;
  }

  S prod(int l, int r) {
    assert(0 <= l && l <= r && r <= _n);
    if (l == r)
      return e();

    node *lptr = p_root;
    node *rptr = p_root;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) {
        lptr = lptr->child[l >> i & 1];
        push(lptr);
      }

      if (((r >> i) << i) != r) {
        rptr = rptr->child[r >> i & 1];
        push(rptr);
      }
    }

    S sml = e(), smr = e();

    bool l_up = false, r_up = false;
    while (l < r) {
      if (l_up)
        lptr = lptr->p;
      if (r_up)
        rptr = rptr->p;

      if (l & 1) {
        l_up = true;
        sml = op(sml, lptr->child[1]->v);
        l++;
      }

      if (r & 1) {
        r_up = true;
        smr = op(rptr->child[0]->v, smr);
        r--;
      }

      l >>= 1;
      r >>= 1;
    }

    return op(sml, smr);
  }

  S all_prod() { return root->v; }

  void apply(int p, F f) {
    assert(0 <= p && p < _n);
    node *ptr = root;
    for (int i = log - 1; i >= 0; i--) {
      push(ptr);
      ptr = ptr->child[p >> i & 1];
    }
    ptr->v = mapping(f, ptr->v);
    for (int i = 1; i <= log; i++) {
      ptr = ptr->p;
      update(ptr);
    }
  }

  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= _n);
    if (l == r)
      return;

    node *lptr = p_root;
    node *rptr = p_root;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) {
        lptr = lptr->child[l >> i & 1];
        push(lptr);
      }

      if (((r >> i) << i) != r) {
        rptr = rptr->child[r >> i & 1];
        push(rptr);
      }
    }

    {
      node *lptr2 = lptr;
      node *rptr2 = rptr;
      bool l_up = false, r_up = false;
      while (l < r) {
        if (l_up)
          lptr = lptr->p;
        if (r_up)
          rptr = rptr->p;

        if (l & 1) {
          l_up = true;
          all_apply(lptr->child[1], f);
          l++;
        }

        if (r & 1) {
          r_up = true;
          all_apply(rptr->child[0], f);
          r--;
        }

        l >>= 1;
        r >>= 1;
      }
      lptr = lptr2;
      rptr = rptr2;
    }

    while (lptr != p_root) {
      update(lptr);
      lptr = lptr->p;
    }
    while (rptr != p_root) {
      rptr = rptr->p;
      update(rptr);
    }
  }

  template<bool (*g)(S)>
  int max_right(int l) {
    return max_right(l, [](S x) { return g(x); });
  }

  template<class G>
  int max_right(int l, G g) {
    assert(0 <= l && l <= _n);
    assert(g(e()));
    if (l == _n)
      return _n;

    l += size;
    node *ptr = p_root;
    int crz = countr_zero(l);
    for (int i = log; i > crz; i--) {
      ptr = ptr->child[l >> i & 1];
      push(ptr);
    }
    l >>= crz;

    S sm = e();
    while (1) {
      if (!g(op(sm, ptr->child[1]->v))) {
        ptr = ptr->child[1];
        while (l < size) {// while (l不是叶子)
          push(ptr);
          if (g(op(sm, ptr->child[0]->v))) {
            sm = op(sm, ptr->child[0]->v);
            l = 2 * l + 1;
            ptr = ptr->child[1];
          } else {
            ptr = ptr->child[0];
            l = 2 * l;
          }
        }
        return l - size;
      }
      sm = op(sm, ptr->child[1]->v);
      l++;
      if ((l & -l) == l)
        break;

      while (l % 2 == 0) {
        l >>= 1;
        ptr = ptr->p;
      }
    }
    return _n;
  }

  template<bool (*g)(S)>
  int min_left(int r) {
    return min_left(r, [](S x) { return g(x); });
  }
  template<class G>
  int min_left(int r, G g) {
    assert(0 <= r && r <= _n);
    assert(g(e()));
    if (r == 0)
      return 0;

    r += size;
    node *ptr = p_root;
    int crz = countr_zero(r);
    for (int i = log; i > crz; i--) {
      ptr = ptr->child[r >> i & 1];
      push(ptr);
    }
    r >>= crz;

    S sm = e();
    while (1) {
      if (!g(op(ptr->child[0]->v, sm))) {
        ptr = ptr->child[0];
        r--;
        while (r < size) {
          push(ptr);
          if (g(op(ptr->child[1], sm))) {
            sm = op(ptr->child[1], sm);
            r = 2 * r;
            ptr = ptr->child[0];
          } else {
            r = 2 * r + 1;
            ptr = ptr->child[1];
          }
        }
        return r - size + 1;
      }
      sm = op(ptr->child[0]->v, sm);
      r--;
      if ((r & -r) == r)
        break;
      while (r % 2 == 0) {
        r >>= 1;
        ptr = ptr->p;
      }
    }
    return 0;
  }

 private:
  int _n, size, log;
  node *root;
  node *p_root;
  void update(node *ptr) { ptr->v = op(ptr->child[0]->v, ptr->child[1]->v); }
  void all_apply(node *ptr, F f) {
    ptr->v = mapping(f, ptr->v);
    ptr->lz = composition(f, ptr->lz);
  }

  void push(node *ptr) {
    if (!ptr->child[0]) {// 若左孩子不存在，创建左孩子和右孩子
      ptr->child[0] = new node(ptr);
      ptr->child[1] = new node(ptr);
    }
    all_apply(ptr->child[0], ptr->lz);
    all_apply(ptr->child[1], ptr->lz);
    ptr->lz = id();
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DYNAMIC_LAZY_SEGTREE_HPP_
