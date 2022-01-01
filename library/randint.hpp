//
// Created by zjs on 2022/1/1.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_RANDINT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_RANDINT_HPP_

#include <random>
#include <chrono>
#include <cassert>

template<typename Int> class randint_generator {
  std::mt19937_64 generator;
  std::uniform_int_distribution<Int> dist;

 public:
  randint_generator(Int a = std::numeric_limits<Int>::min(),
          Int b = std::numeric_limits<Int>::max())
      : generator(std::chrono::steady_clock::now().time_since_epoch().count()),
        dist(a, b) {}
  Int operator()() { return dist(generator); }
};

template<typename Int> Int randint(Int a, Int b) {
  assert(a <= b);
  return randint_generator<Int>(a, b)();
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_RANDINT_HPP_
