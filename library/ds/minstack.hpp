//
// Created by zjs on 2/19/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINSTACK_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINSTACK_HPP_

#include <stack>
#include <utility>
#include <algorithm>

template<typename T> struct minstack {
  std::stack<std::pair<T, T>> st;
  T getmin() { return st.top().second; }
  bool empty() { return st.empty(); }
  int size() { return st.size(); }
  void push(T x) {
    T mn = x;
    if (!empty())
      mn = std::min(mn, getmin());
    st.push({x, mn});
  }
  void pop() { st.pop(); }
  T top() { return st.top().first; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DS_MINSTACK_HPP_
