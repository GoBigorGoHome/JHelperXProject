//
// Created by zjs on 5/17/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_COMPRESS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_COMPRESS_HPP_
#include <vector>
#include <algorithm>
#include <cassert>

template<typename T> class compress {
  std::vector<T> a;
  bool ready = false;

 public:
  compress() = default;
  void add(T v) {
    assert(!ready);
    a.push_back(v);
  }
  void build() {
    assert(!ready);
    std::sort(a.begin(), a.end());
    a.erase(std::unique(a.begin(), a.end()), a.end());
    ready = true;
  }
  int size() const {
    assert(ready);
    return int(a.size());
  }

  int get(int v) const {
    assert(ready);
    auto it = std::lower_bound(a.begin(), a.end(), v);
    assert(it != a.end() && *it == v);
    return int(it - a.begin());
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_STRING_COMPRESS_HPP_
