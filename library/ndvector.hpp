//
// Created by zjsdu on 2021/11/25.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NDVECTOR_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NDVECTOR_HPP_
#include <vector>
#include <tuple_utils.hpp>

// from cuiaoxiang
template<class T> auto vect(const T& v, int n) {
  return std::vector<T>(n, v);
}

template<class T, class... D> auto vect(const T& v, int n, D... m) {
  return std::vector<decltype(vect(v, m...))>(n, vect(v, m...));
}

template<typename T, int... Ints>
auto get_vect(const T& v, std::integer_sequence<int, Ints...>)
    -> decltype(vect(v, Ints...));

template<typename T, int Dimension>
using vector_t =
    decltype(get_vect(T(), std::make_integer_sequence<int, Dimension>()));

template<typename T, int Dimension>
class ndvector : public vector_t<T, Dimension> {
  using base_type = vector_t<T, Dimension>;

  template<typename Tuple1, typename Tuple2, std::size_t... I>
  ndvector(const Tuple1& t1, const Tuple2& t2, std::index_sequence<I...>)
      : base_type(vect(construct_from_tuple<T>(t1), std::get<I>(t2)...)) {}

  template<typename Tuple>
  explicit ndvector(const Tuple& t)
      : ndvector(exclude_first_n_elements<Dimension>(t),
                 first_n_elements<Dimension>(t),
                 std::make_index_sequence<Dimension>{}) {}

 public:
  template<typename... Args>
  explicit ndvector(Args... args) : ndvector(std::make_tuple(args...)) {}

  ndvector(const base_type& a) : base_type(a) {}
};

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NDVECTOR_HPP_
