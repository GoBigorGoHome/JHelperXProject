//
// Created by zjs on 2/19/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINQUEUE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINQUEUE_HPP_

#include <cassert>
#include "minstack.hpp"

template<typename T> class minqueue {
  minstack<T> l, r;
  void move() {
    while (!r.empty()) {
      l.push(r.top());
      r.pop();
    }
  }

 public:
  int size() const { return l.size() + r.size(); }
  bool empty() const { return l.empty() && r.empty(); }
  void push(T x) { r.push(x); }
  void pop() {
    assert(!empty());
    if (l.empty())
      move();
    l.pop();
  }
  T front() const {
    assert(!empty());
    if (l.empty())
      move();
    return l.top();
  }
  T getmin() const {
    assert(!empty());
    if (l.empty())
      return r.getmin();
    if (r.empty())
      return l.getmin();
    return std::min(l.getmin(), r.getmin());
  }
  // If back() is needed, use mindeque instead.
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINQUEUE_HPP_
