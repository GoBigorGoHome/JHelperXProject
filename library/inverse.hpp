//
// Created by zjsdu on 3/7/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_INVERSE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_INVERSE_HPP_

#include <cassert>
#include <utility>// std::swap

template<typename T> T inverse(T a, T m) {
  assert(a != 0);
  assert(m > 0);
  T b = m, u = 0, v = 1;
  while (a != 0) {
    T t = b / a;
    b -= t * a;
    std::swap(a, b);
    u -= t * v;
    std::swap(u, v);
  }
  assert(b == 1);
  return u < 0 ? u + m : u;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_INVERSE_HPP_
