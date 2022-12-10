//
// Created by zjs on 12/11/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MSET_HPP_
#include <set>
#include <cassert>
template<typename T> class mset : public std::multiset<T> {
  using base = std::multiset<T>;
  using base::base;// inherit constructors

 public:
  // 若有多个x，只删除一个。
  bool erase(int x) {
    auto it = base::find(x);
    if (it != base::end()) {
      base::erase(it);
      return 1;
    }
    return 0;
  }

  T max() const {
    assert(!base::empty());
    return *base::rbegin();
  }

  T min() const {
    assert(!base::empty());
    return *base::begin();
  }

  void erase_min() {
    assert(!base::empty());
    base::erase(base::begin());
  }

  void erase_max() {
    assert(!base::empty());
    base::erase(std::prev(base::end()));
  }
};
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MSET_HPP_

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MSET_HPP_
