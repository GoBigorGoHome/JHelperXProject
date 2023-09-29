//
// Created by zjs on 8/31/23.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_INV_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_INV_HPP_
#include <ntt.hpp>
template<typename T> std::vector<T> poly_inv(std::vector<T> a, int degree) {
  assert(!a.empty());
  assert(a[0] != 0);
  std::vector<T> b = {1 / a[0]};
  for (int len = 1; len < degree; len *= 2) {
    // 长度为len*4的DFT
    // g[len](x) * (2 - f[2*len](x)g[len](x))
    // 算DFT[4*len](g) 和 DFT[4*len](f)
  }
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_POLY_INV_HPP_
