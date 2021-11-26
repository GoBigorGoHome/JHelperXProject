//
// Created by zjsdu on 2021/11/25.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_TUPLE_UTILS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_TUPLE_UTILS_HPP_
#include <utility>
#include <tuple>

namespace detail {

template<typename Tuple, std::size_t... I>
auto first_n_elements_impl(const Tuple& t, std::index_sequence<I...>) {
  return std::make_tuple(std::get<I>(t)...);
}

template<std::size_t n, typename Tuple, std::size_t... I>
auto exclude_first_n_elements_impl(const Tuple& t, std::index_sequence<I...>) {
  return std::make_tuple(std::get<n + I>(t)...);
}

template<typename T, typename Tuple, std::size_t... I>
T construct_from_tuple_impl(const Tuple& t, std::index_sequence<I...>) {
  return T(std::get<I>(t)...);
}

}// namespace detail

template<std::size_t n, typename Tuple,
         typename = std::enable_if_t<std::tuple_size_v<Tuple> >= n>>
auto first_n_elements(const Tuple& t) {
  return detail::first_n_elements_impl(t, std::make_index_sequence<n>{});
}

template<std::size_t n, typename Tuple,
         typename = std::enable_if_t<std::tuple_size_v<Tuple> >= n>>
auto exclude_first_n_elements(const Tuple& t) {
  return detail::exclude_first_n_elements_impl<n>(
      t, std::make_index_sequence<std::tuple_size_v<Tuple> - n>{});
}

template<typename T, typename Tuple> T construct_from_tuple(const Tuple& t) {
  return detail::construct_from_tuple_impl<T>(
      t, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_TUPLE_UTILS_HPP_
