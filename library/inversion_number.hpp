//
// Created by zjsdu on 5/23/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_INVERSION_NUMBER_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_INVERSION_NUMBER_HPP_
#include <vector>
#include <map>
#include <fenwick.hpp>
/// @brief Calculate inversion number between two permutations.
/// @param p1, p2 Permutations of a multiset.
template<typename T>
long long inversion_number(const std::vector<T> &p1, const std::vector<T> &p2) {
  if (p1.size() != p2.size())
    return -1;
  std::map<T, std::vector<int>> id;
  int order = 0;
  for (T x : p1) {
    id[x].push_back(order++);
  }
  std::map<T, std::size_t> cnt;
  for (T x : p2) {
    cnt[x]++;
  }
  for (auto [v, c] : cnt) {
    auto it = id.find(v);
    if (it == id.end() or it->second.size() != c) {
      return -1;
    }
  }

  long long result = 0;
  const int n = (int) p1.size();
  BIT<int> b(n);
  for (int i = n - 1; i >= 0; --i) {
    int ID = id[p2[i]][--cnt[p2[i]]];
    result += b.sum(ID);
    b.add(ID, 1);
  }
  return result;
}

/// @param p A permutation of 0, 1, 2, ..., n - 1.
long long inversion_number(const std::vector<int> &p) {
  long long result = 0;
  int n = (int) p.size();
  BIT<int> b(n);
  for (int i = n - 1; i >= 0; --i) {
    result += b.sum(p[i]);
    b.add(p[i], 1);
  }
  return result;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_INVERSION_NUMBER_HPP_
