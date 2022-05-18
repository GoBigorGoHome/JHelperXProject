//
// Created by zjsdu on 10/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TASKS_TYPE_TRAITS_HPP_
#define JHELPER_EXAMPLE_PROJECT_TASKS_TYPE_TRAITS_HPP_
#include <type_traits>
#include <string>

#if __cplusplus >= 201703L
template<typename T> T type();// no definition
template<typename Container> auto value_type_of_() {
  if constexpr (std::is_array_v<Container>)
    return type<std::remove_extent_t<Container>>();
  else
    return type<typename Container::value_type>();
}
template<typename Container>
using value_type_of =
    decltype(value_type_of_<std::remove_reference_t<Container>>());
#else
template <typename Container>
struct value_type_of_impl // default, non-array
{
  using type = typename Container::value_type;
};

template <typename T, std::size_t N>
struct value_type_of_impl<T[N]> // arrays
{
  using type = T;
};

template <typename Container>
using value_type_of = typename value_type_of_impl<Container>::type;
#endif
// Source: https://foonathan.net/2020/10/iife-metaprogramming/

#if __cplusplus >= 201703L
namespace is_iterable_impl {
using std::begin;
using std::end;
template<typename T>
using check_specs = std::void_t<
    std::enable_if_t<
        std::is_same<decltype(begin(std::declval<T &>())),// has begin()
                       decltype(end(std::declval<T &>()))   // has end()
                       >::value>,// ... begin() and end() are the same type ...
    decltype(*begin(std::declval<T &>()))>;// ... which can be dereferenced
template<typename, typename = void> struct is_iterable : std::false_type {};
// specialization
template<class T> struct is_iterable<T, check_specs<T>> : std::true_type {};
}// namespace is_iterable_impl
template<class T> using is_iterable = is_iterable_impl::is_iterable<T>;
template<typename T> constexpr bool is_iterable_v = is_iterable<T>::value;
// Source: https://stackoverflow.com/a/53429396/6793559

template<typename T>
using is_string =
    std::disjunction<std::is_same<char *, typename std::decay_t<T>>,
                     std::is_same<const char *, typename std::decay_t<T>>,
                     std::is_same<std::string, typename std::decay_t<T>>>;
template<typename T> constexpr bool is_string_v = is_string<T>::value;
// Source: https://stackoverflow.com/a/57812868/6793559

template<template<typename...> typename Target, typename Aux, typename... Ts>
struct is_specialized_for_impl : std::false_type {};

template<template<typename...> typename Target, typename... Args>
struct is_specialized_for_impl<Target, decltype(sizeof(Target<Args...>)),
                               Args...> : std::true_type {};

template<template<typename...> typename Target, typename... Args>
using is_specialized_for =
    is_specialized_for_impl<Target, std::size_t, Args...>;
template<template<typename...> typename Target, typename... Args>
constexpr bool is_specialized_for_v =
    is_specialized_for<Target, Args...>::value;

template<typename T>
using is_tuple_like = is_specialized_for<std::tuple_size, T>;
template<typename T> constexpr bool is_tuple_like_v = is_tuple_like<T>::value;

template<typename T, typename = void> struct remove_all_extents_ {
  typedef std::remove_reference_t<T> type;
};

template<typename T>
struct remove_all_extents_<T, std::void_t<decltype(std::declval<T>()[0])>> {
  typedef
      typename remove_all_extents_<decltype(std::declval<T>()[0])>::type type;
};

template<typename T, typename = void>
struct rank_ : public std::integral_constant<std::size_t, 0> {};

template<typename T>
struct rank_<T, std::void_t<decltype(std::declval<T>()[0])>>
    : public std::integral_constant<
          std::size_t, rank_<decltype(std::declval<T>()[0])>::value + 1> {};
#endif
#endif// JHELPER_EXAMPLE_PROJECT_TASKS_TYPE_TRAITS_HPP_
