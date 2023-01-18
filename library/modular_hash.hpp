//
// Created by zjsdu on 10/13/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_MODULAR_HASH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_MODULAR_HASH_HPP_
#include <vector>
#include <cassert>
#include <modular.hpp>

/// @brief A string hash function (the one used in the Rabin-Karp
/// string-matching algorithm) that returns hash value of any substring of the
/// given string in O(1) time.
/// @tparam modulus A modulus chosen for the hash function. It should be a large
/// positive integer.
template<int modulus> class modular_hash {
  int length = 0;
  int radix_;
  using mint = Mint<modulus>;
  std::vector<mint> value;
  std::vector<mint> pow;

 public:
  modular_hash() = default;
  /// @brief Construct the hash function in linear time.
  /// @param a the given string
  /// @param radix the radix chosen for hash function
  template<typename String>
  modular_hash(const String &a, int radix, int offset = 0)
      : length((int) std::size(a)), radix_(radix) {
    assert(length > 0);
    assert(radix > 0);
    value.resize(length);
    pow.resize(length);
    mint sum = 0;
    for (int i = 0; i < length; i++) {
      assert(a[i] >= offset);
      assert(a[i] - offset < radix);
      sum = sum * radix + (a[i] - offset);
      value[i] = sum;
    }
    pow[0] = 1;
    for (int i = 1; i < length; i++) {
      pow[i] = radix * pow[i - 1];
    }
  }
  /// @brief Returns hash value of a substring [\p pos, \p pos + \p count).
  /// @param p position of the first character to include
  /// @param count length of the substring
  mint operator()(int pos, int count) const {
    assert(count >= 0);
    assert(0 <= pos and pos + count <= length);
    if (count == 0)
      return 0;
    return pos == 0 ? value[count - 1]
                    : value[pos + count - 1] - pow[count] * value[pos - 1];
  }
  /// @brief Returns hash value of a given string.
  /// @param key the string to be hashed
  template<typename String>
  mint operator()(const String &key, int radix, int offset = 0) const {
    mint res = 0;
    for (auto x : key) {
      assert(offset <= x and x - offset < radix);
      res = res * radix + (x - offset);
    }
    return res;
  }
  // 返回radix的n次幂。
  mint power(int n) const {
    assert(n >= 0);
    if (n < length)
      return pow[n];
    return Power(mint(radix_), n);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_MODULAR_HASH_HPP_
