//
// Created by zjs on 4/10/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SUFFIX_MAX_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SUFFIX_MAX_HPP_
// 带坐标压缩（aka 离散化）的后缀最大值
// 用 fenwick tree 实现

#include <vector>
#include <algorithm>
#include <cassert>
template<typename Key, typename Value> class suffix_max {
  // identity 单位元，即最小值
  std::vector<Key> keys_;
  Value identity;
  std::vector<Value> a;
  int size;

  void update(int i, Value v) {
    while (i >= 0) {
      a[i] = std::max(a[i], v);
      i = (i & (i + 1)) - 1;
    }
  }

  Value query_(int i) {
    Value ans = identity;
    while (i < size) {
      ans = std::max(ans, a[i]);
      i |= i + 1;
    }
    return ans;
  }

 public:
  suffix_max(const std::vector<Key> keys, Value identity)
      : keys_(keys), identity(identity) {
    std::sort(keys_.begin(), keys_.end());
    keys_.erase(std::unique(keys_.begin(), keys_.end()), keys_.end());
    size = (int) keys_.size();
    a.assign(keys_.size(), identity);
  }

  void insert(Key k, Value v) {
    auto it = std::lower_bound(keys_.begin(), keys_.end(), k);
    assert(it != keys_.end() && *it == k);
    update(int(it - keys_.begin()), v);
  }

  Value query(Key k) {
    int i =
        (int) (std::lower_bound(keys_.begin(), keys_.end(), k) - keys_.begin());
    return query_(i);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SUFFIX_MAX_HPP_
