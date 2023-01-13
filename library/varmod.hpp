//
// Created by zjs on 2022/4/24.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_VARMOD_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_VARMOD_HPP_

#include <vector>
#include <modular.hpp>

using mint = Modular<VarMod>;

mint operator""_m(unsigned long long v) {
  return mint(static_cast<int>(v));
}

template<typename Int> mint power(mint x, Int n) {
  return Power(x, n);
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_VARMOD_HPP_
