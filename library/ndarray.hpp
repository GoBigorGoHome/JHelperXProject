//
// Created by zjsdu on 10/26/2020.
//
#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
#include <vector>
// arbitrary-dimensional array that allows non-constexpr extents.
// Usage: v<dimension, value_type> arr(extents..., init_value);
// All extensions but one for the last dimension are required. An initial value
// for all array items can be specified if all extensions are present.
// Examples:
// vec<2, int> a(2, 3, -1); vec<3, int> b(2, 3, 4); vec<3, int> c(2, 3);
template<unsigned dimension, typename T>
class vec : public std::vector<vec<dimension - 1, T>> {
 public:
  template<typename... Args>
  vec(unsigned d, Args... args)
      : std::vector<vec<dimension - 1, T>>(d, vec<dimension - 1, T>(args...)) {}
};
template<typename T> class vec<1, T> : public std::vector<T> {
 public:
  template<typename... Args>
  vec<1, T>(Args &&...args) : std::vector<T>(std::forward<Args>(args)...) {}
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
