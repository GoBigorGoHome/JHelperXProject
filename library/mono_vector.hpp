//
// Created by zjsdu on 3/23/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MONO_VECTOR_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MONO_VECTOR_HPP_
#include <vector>
#include <functional>
/// @brief Monotonic vector (a.k.a. monotonic stack).
template<typename T, typename Compare>
class mono_vector : public std::vector<std::pair<T, int>> {
 private:
  Compare cmp;

 public:
  void push_back(const T &value, int i) {
    while (not this->empty() and not cmp(this->back().first, value)) {
      this->pop_back();
    }
    this->emplace_back(value, i);
  }
};
/// @brief Strictly increasing vector.
template<typename T> using inc_vector = mono_vector<T, std::less<T>>;
/// @brief Strictly decreasing vector.
template<typename T> using dec_vector = mono_vector<T, std::greater<T>>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MONO_VECTOR_HPP_
