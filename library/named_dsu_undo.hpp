//
// Created by zjsdu on 2022/3/15.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NAMED_DSU_UNDO_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NAMED_DSU_UNDO_HPP_

#include <vector>
#include <stack>
#include <cassert>

/// @brief A data structure for disjoint set union of named sets. Supports undo
/// of union.
class named_dsu_undo {
  std::vector<int> parent_or_name;
  std::vector<int> root;
  std::vector<int> size;
  struct record {
    int a, root_a, size_a, b, root_b, size_b;
  };
  std::stack<record> history;

 public:
  /// @param n number of elements
  /// @param m number of names
  /// Elements and names are 0-indexed.
  named_dsu_undo(int n, int m) : parent_or_name(n), root(m, -1), size(m, 0) {}

  /// Return the name of the unique set containing the element x, or -1 if no
  /// named set contains x.
  int name(int x) const {
    while (parent_or_name[x] >= 0)
      x = parent_or_name[x];
    return -(parent_or_name[x] + 1);
  }

  /// Combine the two sets A and B into a new set named A.
  void merge(int A, int B) {
    history.push({A, root[A], size[A], B, root[B], size[B]});
    if (A == B or root[B] == -1)
      return;
    int x = root[A], y = root[B];
    if (size[A] < size[B]) {
      std::swap(x, y);
    }
    if (y != -1)
      parent_or_name[y] = x;
    parent_or_name[x] = -(A + 1);
    root[A] = x;
    root[B] = -1;
    size[A] += size[B];
    size[B] = 0;
  }

  /// Undo the last merge operation.
  void undo() {
    record t = history.top();
    history.pop();
    if (t.a == t.b or t.root_b == -1)
      return;
    root[t.a] = t.root_a;
    root[t.b] = t.root_b;
    size[t.a] = t.size_a;
    size[t.b] = t.size_b;
    if (t.root_a != -1)
      parent_or_name[t.root_a] = -(t.a + 1);
    parent_or_name[t.root_b] = -(t.b + 1);
  }

  /// Add an element x to the set named name.
  void add_to_set(int x, int name) {
    if (root[name] == -1) {
      root[name] = x;
      parent_or_name[x] = -(name + 1);
    } else {
      parent_or_name[x] = root[name];
    }
    size[name] += 1;
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NAMED_DSU_UNDO_HPP_
