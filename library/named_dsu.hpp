//
// Created by zjs on 11/26/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NAMED_DSU_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NAMED_DSU_HPP_
#include <vector>
#include <cassert>
// This class is written for https://atcoder.jp/contests/abc279/tasks/abc279_f
// Sets and elements are 1-indexed.
class named_dsu {
  std::vector<int> parent_or_name, leader;
  int size;
  const int n_sets;

  int root(int x) {
    return parent_or_name[x] < 0 ? x
                                 : parent_or_name[x] = root(parent_or_name[x]);
  }

 public:
  // leader[i]：集合i里的代表元
  named_dsu(int n_sets)
      : parent_or_name(1), leader(n_sets + 1), size(0), n_sets(n_sets) {}

  void merge(int x, int y) {// Put all contents of set y into set x.
    assert(1 <= x && x <= n_sets);
    assert(1 <= y && y <= n_sets);
    if (y == x || leader[y] == 0)
      return;
    if (leader[x] == 0) {
      leader[x] = leader[y];
      parent_or_name[leader[x]] = -x;
    } else {
      parent_or_name[leader[y]] = leader[x];
    }
    leader[y] = 0;
  }

  // Returns which set contains element x.
  int set_id(int x) {
    assert(1 <= x && x <= size);
    return -parent_or_name[root(x)];
  }

  void add_to_set(int i) {// Add the next element to set i.
    size++;
    if (leader[i] == 0) {
      leader[i] = size;
      parent_or_name.push_back(-i);
    } else {
      parent_or_name.push_back(leader[i]);
    }
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NAMED_DSU_HPP_
