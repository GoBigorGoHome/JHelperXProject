//
// Created by zjs on 2/19/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINDEQUE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINDEQUE_HPP_
#include "minstack.hpp"

template<typename T> struct mindeque {
  minstack<T> l, r;
  std::stack<T> t;

  void rebalance() {
    bool f = false;
    if (r.empty()) {
      f = true;
      std::swap(l, r);
    }
    int sz = r.size() / 2;
    while (sz--) {
      t.push(r.top());
      r.pop();
    }
    while (!r.empty()) {
      l.push(r.top());
      r.pop();
    }
    while (!t.empty()) {
      r.push(t.top());
      t.pop();
    }
    if (f)
      std::swap(l, r);
  }
  T getmin() {
    if (l.empty())
      return r.getmin();
    if (r.empty())
      return l.getmin();
    return min(l.getmin(), r.getmin());
  }
  bool empty() { return l.empty() && r.empty(); }
  int size() { return l.size() + r.size(); }
  void push_front(T x) { l.push(x); }
  void push_back(T x) { r.push(x); }
  void pop_front() {
    if (l.empty())
      rebalance();
    l.pop();
  }
  void pop_back() {
    if (r.empty())
      rebalance();
    r.pop();
  }
  T front() {
    if (l.empty())
      rebalance();
    return l.top();
  }
  T back() {
    if (r.empty())
      rebalance();
    return r.top();
  }
};

template<typename T> void swap(mindeque<T> &x, mindeque<T> &y) {
  std::swap(x.l, y.l);
  std::swap(x.r, y.r);
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINDEQUE_HPP_
