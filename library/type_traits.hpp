//
// Created by zjsdu on 10/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TASKS_TYPE_TRAITS_HPP_
#define JHELPER_EXAMPLE_PROJECT_TASKS_TYPE_TRAITS_HPP_
#include <type_traits>
template<typename T> T type();// no definition
template<typename Container> auto value_type_of_() {
  if constexpr (std::is_array_v<Container>)
    return type<std::remove_extent_t<Container>>();
  else
    return type<typename Container::value_type>();
}
template<typename Container>
using value_type_of = decltype(value_type_of_<Container>());
// Source: https://foonathan.net/2020/10/iife-metaprogramming/

namespace is_iterable_impl {
using std::begin, std::end;
template<typename T>
using check_specs = std::void_t<
    std::enable_if_t<
        std::is_same_v<decltype(begin(std::declval<T &>())),// has begin()
                       decltype(end(std::declval<T &>()))   // has end()
                       >>,// ... begin() and end() are the same type ...
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
#endif// JHELPER_EXAMPLE_PROJECT_TASKS_TYPE_TRAITS_HPP_
