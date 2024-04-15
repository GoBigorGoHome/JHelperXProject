//
// Created by zjs on 4/15/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_COORDINATE_COMPRESS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_COORDINATE_COMPRESS_HPP_

#include <vector>
#include <algorithm>
#include <numeric>

// 坐标压缩
template<typename T>
std::vector<int> compress(const std::vector<T>& a) {
  int n = (int) a.size();
  std::vector<int> I(n);
  std::iota(I.begin(), I.end(), 0);
  std::sort(I.begin(), I.end(),
            [&a](int i, int j) { return a[i] < a[j]; });
  std::vector<int> order(n);
  for (int i = 1; i < n; i++)
    order[I[i]] = order[I[i - 1]] + (a[I[i - 1]] < a[I[i]]);
  return order;
}

// 坐标压缩并且unique
template<typename T>
std::vector<int> compress_unique(std::vector<T>& a) {
  int n = (int) a.size();
  std::vector<int> I(n);
  std::iota(I.begin(), I.end(), 0);
  std::sort(I.begin(), I.end(),
            [&a](int i, int j) { return a[i] < a[j]; });
  std::vector<int> order(n);
  std::vector<T> b(n);
  b[0] = a[I[0]];
  int j = 0;
  for (int i = 1; i < n; i++) {
    if (a[I[i - 1]] < a[I[i]])
      b[++j] = a[I[i]];
    order[I[i]] = j;
  }
  b.erase(b.begin() + j + 1, b.end());
  a = b;
  return order;
}

#endif //JHELPER_EXAMPLE_PROJECT_LIBRARY_COORDINATE_COMPRESS_HPP_
