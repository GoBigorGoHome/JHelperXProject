//
// Created by zjs on 2021/11/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MONOTONIC_QUEUE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MONOTONIC_QUEUE_HPP_
#include <deque>
#include <cassert>

/// @brief A monotonic queue is an abstract data type with similar semantics to
/// that of a \c std::priority_queue. When an element e is to be inserted into
/// the queue, all existing elements "less" than e are eliminated. Early added
/// elements may expire.
/// @tparam T The type of the stored elements.
/// @tparam Compare A strict weak ordering on elements of \p T. Similar to
/// \c std::priority_queue, the front of the queue contains the "last" element
/// according to the given strict weak ordering.

template<typename T, typename Compare = std::less<T>> class monotonic_queue {
  using value_type = typename std::deque<T>::value_type;
  using reference = typename std::deque<T>::reference;
  using const_reference = typename std::deque<T>::const_reference;
  using size_type = typename std::deque<T>::size_type;
  using container_type = std::deque<T>;
  using value_compare = Compare;

  std::deque<T> q;
  Compare comp;

 public:
  monotonic_queue() : q(), comp() {}

  explicit monotonic_queue(const Compare& comp) : q(), comp(comp) {}

  void push(const value_type& val) {
    while (not q.empty() and comp(q.back(), val)) {
      q.pop_back();
    }
    q.push_back(val);
  }

  void pop(const value_type& expired_value) {
    if (not q.empty() and not comp(expired_value, q.front())) {
      q.pop_front();
    }
  }

  void push_pop(const value_type& value, const value_type& expired_value) {
    push(value);
    pop(expired_value);
  }

  void clear() { q.clear(); }

  const_reference top() const {
    assert(not q.empty());
    return q.front();
  }

  size_type size() const { return q.size(); }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MONOTONIC_QUEUE_HPP_
