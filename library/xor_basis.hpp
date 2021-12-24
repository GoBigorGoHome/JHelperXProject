//
// Created by zjs on 2021/10/19.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
#include <array>
#include <vector>
#include <cassert>

/// @brief Maintains a linearly independent list of vectors (hereinafter the
/// basis) in a vector space over the field F2. A vector is represented as an
/// integer of type \p T. The dimension of the vector space is \p Dimension.
template<typename T, std::size_t Dimension> class xor_basis {
  // std_basis is another linearly independent list of vectors in echelon form.
  std::array<T, Dimension> std_basis_ = {};
  std::array<T, Dimension> real_basis = {};
  // write std_basis as a linear combination of real_basis.
  std::array<T, Dimension> std_in_real = {};
  std::array<int, Dimension> order;
  int dim = 0;
  int cnt = 0;

 public:
  xor_basis() = default;

  /// @brief Add a vector \p x to the basis, if \p x is a linear combination of
  /// the current basis, remove the most recent added vector from the basis.
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

  /// @brief Returns a subset of the basis whose sum is the vector \p x.
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

  /// @brief Returns when each of the components of vector \p x were added to
  /// basis. This method is not useful in general, it is for the problem abc223h
  /// XOR Query.
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
      if (mask >> i & 1) {
        result.push_back(order[i]);
      }
    }
    return result;
  }

  // Returns whether a vector \p x is in the span of the basis.
  bool contains(T x) const {
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
