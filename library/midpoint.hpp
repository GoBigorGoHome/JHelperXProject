//
// Created by Administrator on 12/2/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MIDPOINT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MIDPOINT_HPP_
#include <type_traits>
//! @brief An implementation of C++20's std::midpoint on integral types other
//! than bool.
//! @return Half the sum of a and b. If the sum is odd, the result it rounded
//! towards a.
//! @remark No overflow occurs.
template<typename Int> constexpr Int midpoint(Int a, Int b) noexcept {
  using U = std::make_unsigned_t<Int>;
  int sign = 1;
  U m = a;
  U M = b;
  if (a > b) {
    sign = -1;
    m = b;
    M = a;
  }
  return a + sign * Int(U(M - m) >> 1);
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MIDPOINT_HPP_
