//
// Created by zjs on 6/26/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_MASPY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_MASPY_HPP_
#include <bit.hpp>
template<int LOG, bool PERSISTENT, int NODES,
         typename UINT = unsigned long long, typename SIZE_TYPE = int>
struct binary_trie {
  using T = SIZE_TYPE;
  struct Node {
    int width;
    UINT val;
    T cnt;
    Node *l, *r;
  };

  Node *pool;
  int pid;
  using np = Node *;

  binary_trie() : pid(0) { pool = new Node[NODES]; }

  ~binary_trie() { delete[] pool; }
  void reset() { pid = 0; }

  np new_root() { return nullptr; }

  np add(np root, UINT val, T cnt = 1) {
    if (!root)
      root = new_node(0, 0);
    assert(0 <= val && val < (1LL << LOG));
    return add_rec(root, LOG, val, cnt);
  }

  // f(val, cnt)
  template<typename F> void enumerate(np root, F f) {
    auto dfs = [&](auto &dfs, np root, UINT val, int ht) -> void {
      if (ht == 0) {
        f(val, root->cnt);
        return;
      }
      np c = root->l;
      if (c) {
        dfs(dfs, c, val << (c->width) | (c->val), ht - (c->width));
      }
      c = root->r;
      if (c) {
        dfs(dfs, c, val << (c->width) | (c->val), ht - (c->width));
      }
    };
    if (root)
      dfs(dfs, root, 0, LOG);
  }

  // xor_val したあとの値で昇順 k 番目
  UINT kth(np root, T k, UINT xor_val) {
    assert(root && 0 <= k && k < root->cnt);
    return kth_rec(root, 0, k, LOG, xor_val) ^ xor_val;
  }

  // xor_val したあとの値で最小値
  UINT min(np root, UINT xor_val) {
    assert(root && root->cnt);
    return kth(root, 0, xor_val);
  }

  // xor_val したあとの値で最大値
  UINT max(np root, UINT xor_val) {
    assert(root && root->cnt);
    return kth(root, (root->cnt) - 1, xor_val);
  }

  // 和 xor_val 异或的结果在 [0, upper) 内的数的个数
  T count(np root, UINT upper, UINT xor_val) {
    if (!root)
      return 0;
    return prefix_count_rec(root, LOG, upper, xor_val, 0);
  }

  // 以key的后len位为前缀的数的个数
  T count_prefix(np root, UINT key, int len) const {
    assert(len >= 0);
    if (!root)
      return 0;
    if (len == 0)
      return root->cnt;
    bool go_r = (key >> (len - 1)) & 1;
    np c = go_r ? root->r : root->l;
    if (!c)
      return 0;
    int w = c->width;
    if (len <= w) {
      if (c->val >> (w - len) == key)
        return c->cnt;
      return 0;
    }
    // len > w
    if ((key >> (len - w)) == c->val)
      return count_prefix(c, key & mask(len - w), len - w);
    return 0;
  }

  // 和 xor_val 异或的结果在 [lo, hi) 内的数的个数
  T count(np root, UINT lo, UINT hi, UINT xor_val) {
    return count(root, hi, xor_val) - count(root, lo, xor_val);
  }

 private:
  inline UINT mask(int k) const { return (UINT(1) << k) - 1; }

  np new_node(int width, UINT val) {
    pool[pid].l = pool[pid].r = nullptr;
    pool[pid].width = width;
    pool[pid].val = val;
    pool[pid].cnt = 0;
    return &(pool[pid++]);
  }

  np copy_node(np c) {
    if (!c || !PERSISTENT)
      return c;
    np res = &(pool[pid++]);
    res->width = c->width, res->val = c->val;
    res->cnt = c->cnt, res->l = c->l, res->r = c->r;
    return res;
  }

  np add_rec(np root, int ht, UINT val, T cnt) {
    root = copy_node(root);
    root->cnt += cnt;
    if (ht == 0)
      return root;

    bool go_r = (val >> (ht - 1)) & 1;
    np c = (go_r ? root->r : root->l);
    if (!c) {
      c = new_node(ht, val);
      c->cnt = cnt;
      if (!go_r)
        root->l = c;
      if (go_r)
        root->r = c;
      return root;
    }
    int w = c->width;
    if ((val >> (ht - w)) == c->val) {
      c = add_rec(c, ht - w, val & mask(ht - w), cnt);
      if (!go_r)
        root->l = c;
      if (go_r)
        root->r = c;
      return root;
    }
    // 计算 value 和 c->value 的最长公共前缀的长度
    // val的长度是ht，c->value的长度是w，且ht>=w
    int same = w - 1 - topbit((val >> (ht - w)) ^ (c->val));
    np n = new_node(same, (c->val) >> (w - same));
    n->cnt = c->cnt + cnt;
    c = copy_node(c);
    c->width = w - same;
    c->val = c->val & mask(w - same);
    if ((val >> (ht - same - 1)) & 1) {
      n->l = c;
      n->r = new_node(ht - same, val & mask(ht - same));
      n->r->cnt = cnt;
    } else {
      n->r = c;
      n->l = new_node(ht - same, val & mask(ht - same));
      n->l->cnt = cnt;
    }
    if (!go_r)
      root->l = n;
    if (go_r)
      root->r = n;
    return root;
  }

  UINT kth_rec(np root, UINT val, T k, int ht, UINT xor_val) {
    if (ht == 0)
      return val;
    np left = root->l, right = root->r;
    if ((xor_val >> (ht - 1)) & 1)
      swap(left, right);
    T sl = (left ? left->cnt : 0);
    np c;
    if (k < sl) {
      c = left;
    }
    if (k >= sl) {
      c = right, k -= sl;
    }
    int w = c->width;
    return kth_rec(c, val << w | (c->val), k, ht - w, xor_val);
  }

  T prefix_count_rec(np root, int ht, UINT LIM, UINT xor_val, UINT val) {
    UINT now = (val << ht) ^ (xor_val);
    if ((LIM >> ht) > (now >> ht))
      return root->cnt;
    if (ht == 0 || (LIM >> ht) < (now >> ht))
      return 0;
    T res = 0;
    for (np c : {root->l, root->r}) {
      if (c) {
        int w = c->width;
        res += prefix_count_rec(c, ht - w, LIM, xor_val, val << w | c->val);
      }
    }
    return res;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BINARY_TRIE_MASPY_HPP_
