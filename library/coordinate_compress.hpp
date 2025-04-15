//
// Created by zjs on 4/15/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_COORDINATE_COMPRESS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_COORDINATE_COMPRESS_HPP_

#include <vector>
#include <algorithm>
#include <numeric>

/// 坐标压缩。
/// return a compressed version of a, i.e. maps every element of a to an
/// integer, starting from 0, preserving order.
template<typename T>
std::vector<int> compress(const std::vector<T> &a) {
  int n = (int) a.size();
  std::vector<int> I(n);
  std::iota(I.begin(), I.end(), 0);
  std::sort(I.begin(), I.end(), [&a](int i, int j) { return a[i] < a[j]; });
  std::vector<int> order(n);
  for (int i = 1; i < n; i++)
    order[I[i]] = order[I[i - 1]] + (a[I[i - 1]] < a[I[i]]);
  return order;
}

/// return a compressed version of a, relative to b, i.e. maps each a[i] to
/// number of **distinct** elements of b that is less than a[i].
template<typename T>
std::vector<int> compress(const std::vector<T> &a, std::vector<T> b) {
  std::sort(b.begin(), b.end());
  auto end = std::unique(b.begin(), b.end());
  std::vector<int> res(a.size());
  for (int i = 0; i < (int) a.size(); i++)
    res[i] = (int) (std::lower_bound(b.begin(), end, a[i]) - b.begin());
  return res;
}

/// return a compressed version of a, make a sorted and remove duplicated
/// elements of a.
template<typename T>
std::vector<int> compress_unique(std::vector<T> &a) {
  int n = (int) a.size();
  std::vector<int> I(n);
  std::iota(I.begin(), I.end(), 0);
  std::sort(I.begin(), I.end(), [&a](int i, int j) { return a[i] < a[j]; });
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

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_COORDINATE_COMPRESS_HPP_
