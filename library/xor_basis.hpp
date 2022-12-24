//
// Created by zjs on 12/24/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
#include <array>
#include <vector>
#include <cassert>

/// @brief Maintains a linearly independent list of vectors (hereinafter the
/// basis) in a vector space over the field F2. A vector is represented as an
/// integer of type \p T. The dimension of the vector space is \p Dimension.
template<typename T, int Dimension> class xor_basis {
  static_assert(Dimension > 0);
  // std_basis is another linearly independent list of vectors in echelon form.
  std::array<T, Dimension> std_basis_ = {};
  std::array<T, Dimension> real_basis = {};
  // write std_basis as a linear combination of real_basis.
  std::array<T, Dimension> std_in_real = {};
  int dim = 0;
  bool normalized = false;

 public:
  xor_basis() = default;

  /// @brief Add a vector \p x to the basis.
  void add(T x) {
    if (x == 0)
      return;
    T y = x;
    T vec = 0;
    for (int i = Dimension - 1; i >= 0; i--) {
      if (y >> i & 1) {
        if (std_basis_[i] == 0) {
          std_basis_[i] = y;
          std_in_real[i] = vec ^ ((T) 1 << dim);
          real_basis[dim] = x;
          dim += 1;
          return;
        }
        y ^= std_basis_[i];
        vec ^= std_in_real[i];
      }
    }
  }
  void normalize_std_basis() {
    if (normalized)
      return;
    normalized = true;
    for (int i = 0; i < Dimension; i++)
      if (std_basis_[i])
        for (int j = i + 1; j < Dimension; j++)
          if (std_basis_[j] >> i & 1) {
            std_basis_[j] ^= std_basis_[i];
            std_in_real[j] ^= std_in_real[i];
          }
  }

  /// @brief Returns a subset of the basis whose sum is the vector \p x.
  std::vector<T> components(T x) const {
    assert(x > 0);
    T mask = 0;
    for (int i = 0; i < Dimension; i++) {
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

  auto std_basis() const {
    std::vector<T> ans;
    for (int i = 0; i < Dimension; i++)
      if (std_basis_[i])
        ans.push_back(std_basis_[i]);
    return ans;
  }

  int dimension() const { return dim; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_XOR_BASIS_HPP_
