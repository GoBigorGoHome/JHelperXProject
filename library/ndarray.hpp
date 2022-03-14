//
// Created by zjsdu on 10/26/2020.
//
#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
#include <vector>
template<typename T, unsigned Dimension> struct ndvec {
  using type = std::vector<typename ndvec<T, Dimension - 1>::type>;
};

template<typename T> struct ndvec<T, 0> { using type = T; };

// arbitrary-dimensional array that allows non-constexpr extents.
// Usage: ndarray<dimension, value_type> arr(extents..., init_value);
// An initial value for all array items can be specified if all extensions are
// specified.
// Examples:
// ndarray<2, int> a(2, 3, -1);
// ndarray<3, int> b(2, 3, 4);
// ndarray<3, int> c(2, 3);
template<unsigned dimension, typename T>
class ndarray : public ndvec<T, dimension>::type {
  using base_type = typename ndvec<T, dimension>::type;
  using value_type = typename base_type::value_type;
  using base_type::base_type;

 public:
  template<typename... Args>
  ndarray(unsigned d, Args... args)
      : std::vector<value_type>(d, ndarray<dimension - 1, T>(args...)) {}
};

template<typename T> class ndarray<1, T> : public std::vector<T> {
  using std::vector<T>::vector;
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NDARRAY_HPP_
