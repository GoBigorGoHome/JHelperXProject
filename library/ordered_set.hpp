//
// Created by zjsdu on 6/9/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_ORDERED_SET_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_ORDERED_SET_HPP_
#include <ext/pb_ds/assoc_container.hpp>
template<typename T>
using ordered_set =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

template<typename T> class ordered_multiset {
  ordered_set<std::pair<T, int>> data;
  int order_ = 0;

 public:
  void insert(const T &x) { data.insert({x, order_++}); }
  // Erase only one (if there is any) occurrence of x.
  void erase(const T &x) {
    auto it = data.lower_bound({x, 0});
    if (it != data.end() and it->first == x) {
      data.erase(it);
    }
  }
  T find_by_order(std::size_t order) const {
    assert(order < data.size());
    return data.find_by_order(order)->first;
  }
  std::size_t order_of_key(const T &key) const {
    return data.order_of_key({key, 0});
  }
  bool empty() const {
    return data.empty();
  }
  std::size_t size() const {
    return data.size();
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_ORDERED_SET_HPP_
