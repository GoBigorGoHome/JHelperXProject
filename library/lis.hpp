//
// Created by zjs on 5/22/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_LIS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_LIS_HPP_

#include <vector>
#include <algorithm>

//最长严格上升子序列。
template<typename T>
int lis(const std::vector<T>& a) {
  std::vector<T> u;
  for (const T& x : a) {
    auto it = std::lower_bound(u.begin(), u.end(), x);
    if (it == u.end()) {
      u.push_back(x);
    } else {
      *it = x;
    }
  }
  return (int) u.size();
}

#endif //JHELPER_EXAMPLE_PROJECT_LIBRARY_LIS_HPP_
