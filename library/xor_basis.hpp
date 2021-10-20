//
// Created by zjs on 2021/10/19.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
#include <array>
#include <vector>
#include <cassert>
template<typename T, std::size_t Dimension> class xor_basis {
  std::array<T, Dimension> std_basis_ = {};
  std::array<T, Dimension> real_basis = {};
  std::array<T, Dimension> std_in_real = {};
  std::array<int, Dimension> order;
  int dim = 0;
  int cnt = 0;

 public:
  xor_basis() = default;
  void add(T x) {
    if (x == 0)
      return;
    T y = x;
    T vec = 0;
    for (std::size_t i = 0; i < Dimension; i++) {
      if (y >> i & 1) {
        if (std_basis_[i] == 0) {
          std_basis_[i] = y;
          std_in_real[i] = vec ^ ((T) 1 << dim);
          real_basis[dim] = x;
          order[dim] = cnt++;
          dim += 1;
          return;
        }
        y ^= std_basis_[i];
        vec ^= std_in_real[i];
      }
    }
    int min_order = cnt;
    int id = -1;
    for (int i = 0; i < dim; i++) {
      if ((vec >> i & 1) and order[i] < min_order) {
        min_order = order[i];
        id = i;
      }
    }
    order[id] = cnt++;
    real_basis[id] = x;
    const T mask = vec ^ ((T) 1 << id);
    for (std::size_t i = 0; i < Dimension; i++) {
      if (std_in_real[i] >> id & 1) {
        std_in_real[i] ^= mask;
      }
    }
  }

  std::vector<T> components(T x) const {
    assert(x > 0);
    T mask = 0;
    for (std::size_t i = 0; i < Dimension; i++) {
      if (x >> i & 1) {
        if (std_basis_[i] == 0) {
          return {};
        }
        x ^= std_basis_[i];
        mask ^= std_in_real[i];
      }
    }
    std::vector<T> result;
    for (int i = 0; i < dim; i++) {
      if (mask >> i & 1) {
        result.push_back(real_basis[i]);
      }
    }
    return result;
  }

  std::vector<int> component_orders(T x) const {
    assert(x > 0);
    T mask = 0;
    for (std::size_t i = 0; i < Dimension; i++) {
      if (x >> i & 1) {
        if (std_basis_[i] == 0) {
          return {};
        }
        x ^= std_basis_[i];
        mask ^= std_in_real[i];
      }
    }
    std::vector<int> result;
    for (int i = 0; i < dim; i++) {
      assert(i < Dimension);
      if (mask >> i & 1) {
        result.push_back(order[i]);
      }
    }
    return result;
  }

  bool is_in_span(T x) const {
    for (std::size_t i = 0; i < Dimension; i++) {
      if (x >> i & 1) {
        if (std_basis_[i] == 0) {
          return false;
        }
        x ^= std_basis_[i];
      }
    }
    return true;
  }

  std::vector<T> basis() const {
    return {real_basis.begin(), real_basis.begin() + dim};
  }

  auto std_basis() const { return std_basis_; }

  int dimension() const { return dim; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
