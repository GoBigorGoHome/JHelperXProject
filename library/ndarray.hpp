//
// Created by zjsdu on 10/26/2020.
//
#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
#include <vector>
// arbitrary-dimensional array that allows non-constexpr extents.
// Usage: ndarray<dimension, value_type> arr(extents..., init_value);
// All extensions but one for the last dimension are required. An initial value
// for all array items can be specified if all extensions are present.
// Examples:
// ndarray<2, int> a(2, 3, -1);
// ndarray<3, int> b(2, 3, 4);
// ndarray<3, int> c(2, 3);
template<unsigned dimension, typename T>
class ndarray : public std::vector<ndarray<dimension - 1, T>> {
 public:
  template<typename... Args>
  ndarray(unsigned d, Args... args)
      : std::vector<ndarray<dimension - 1, T>>(
          d, ndarray<dimension - 1, T>(args...)) {}
};
template<typename T> class ndarray<1, T> : public std::vector<T> {
 public:
  template<typename... Args>
  ndarray<1, T>(Args &&...args) : std::vector<T>(std::forward<Args>(args)...) {}
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
