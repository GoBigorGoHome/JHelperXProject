//
// Created by zjsdu on 7/1/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_NTT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_NTT_HPP_
#include <modular.hpp>
#include <vector>
template<typename T> class NTT {
 public:
  using Type = typename std::decay<decltype(T::value)>::type;
  // md must be a prime.
  static Type md;
  static Modular<T> root;
  static int base;
  static int max_base;
  static std::vector<Modular<T>> roots;
  static std::vector<int> rev;
  static void clear() {
    root = 0;
    base = 0;
    max_base = 0;
    roots.clear();
    rev.clear();
  }
  static void init() {
    md = T::value;
    assert(md >= 3 && md % 2 == 1);
    auto tmp = md - 1;
    max_base = 0;
    // md - 1 == c * 2 ^ max_base
    while (tmp % 2 == 0) {
      tmp /= 2;
      max_base++;
    }
    root = 2;
    // 这里是如何计算域的生成元的？
    while (power(root, (md - 1) >> 1) == 1) {
      root++;
    }
    assert(power(root, md - 1) == 1);
    root = power(root, (md - 1) >> max_base);
    base = 1;
    rev = {0, 1};
    roots = {0, 1};
  }
  static void ensure_base(int nbase) {
    if (md != T::value) {
      clear();
    }
    if (roots.empty()) {
      init();
    }
    if (nbase <= base) {
      return;
    }
    assert(nbase <= max_base);
    // rev[i] := bit_reverse(i)
    rev.resize(1 << nbase);
    for (int i = 0; i < (1 << nbase); i++) {
      rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
    }
    roots.resize(1 << nbase);
    // 计算长度为2k的变换，需要用到一个2k阶元的0到k-1次方
    // roots[k+i] := 2k阶元的i次方
    while (base < nbase) {
      Modular<T> z = power(root, 1 << (max_base - 1 - base));
      for (int i = 1 << (base - 1); i < (1 << base); i++) {
        roots[i << 1] = roots[i];
        roots[(i << 1) + 1] = roots[i] * z;
      }
      base++;
    }
  }
  static void fft(std::vector<Modular<T>> &a) {
    int n = (int) a.size();
    // n is an exact power of 2.
    assert((n & (n - 1)) == 0);
    int zeros = __builtin_ctz(n);
    ensure_base(zeros);
    int shift = base - zeros;
    for (int i = 0; i < n; i++) {
      if (i < (rev[i] >> shift)) {
        swap(a[i], a[rev[i] >> shift]);
      }
    }
    for (int k = 1; k < n; k <<= 1) {
      for (int i = 0; i < n; i += 2 * k) {
        for (int j = 0; j < k; j++) {
          Modular<T> x = a[i + j];
          Modular<T> y = a[i + j + k] * roots[j + k];
          a[i + j] = x + y;
          a[i + j + k] = x - y;
        }
      }
    }
  }
  static std::vector<Modular<T>> multiply(std::vector<Modular<T>> a,
                                          std::vector<Modular<T>> b) {
    if (a.empty() || b.empty()) {
      return {};
    }
    int eq = (a == b);
    int need = (int) a.size() + (int) b.size() - 1;
    int nbase = 0;
    while ((1 << nbase) < need)
      nbase++;
    ensure_base(nbase);
    int sz = 1 << nbase;
    a.resize(sz);
    b.resize(sz);
    fft(a);
    if (eq)
      b = a;
    else
      fft(b);
    Modular<T> inv_sz = 1 / static_cast<Modular<T>>(sz);
    for (int i = 0; i < sz; i++) {
      a[i] *= b[i] * inv_sz;
    }
    reverse(a.begin() + 1, a.end());
    fft(a);
    a.resize(need);
    return a;
  }
};
template<typename T> typename NTT<T>::Type NTT<T>::md;
template<typename T> Modular<T> NTT<T>::root;
template<typename T> int NTT<T>::base;
template<typename T> int NTT<T>::max_base;
template<typename T> std::vector<Modular<T>> NTT<T>::roots;
template<typename T> std::vector<int> NTT<T>::rev;

template<typename T>
std::vector<Modular<T>> inverse(const std::vector<Modular<T>> &a) {
  assert(!a.empty());
  int n = (int) a.size();
  std::vector<Modular<T>> b = {1 / a[0]};
  while ((int) b.size() < n) {
    std::vector<Modular<T>> x(a.begin(),
                              a.begin() + min(a.size(), b.size() << 1));
    x.resize(b.size() << 1);
    b.resize(b.size() << 1);
    std::vector<Modular<T>> c = b;
    NTT<T>::fft(c);
    NTT<T>::fft(x);
    Modular<T> inv = 1 / static_cast<Modular<T>>((int) x.size());
    for (int i = 0; i < (int) x.size(); i++) {
      x[i] *= c[i] * inv;
    }
    reverse(x.begin() + 1, x.end());
    NTT<T>::fft(x);
    rotate(x.begin(), x.begin() + (x.size() >> 1), x.end());
    fill(x.begin() + (x.size() >> 1), x.end(), 0);
    NTT<T>::fft(x);
    for (int i = 0; i < (int) x.size(); i++) {
      x[i] *= c[i] * inv;
    }
    reverse(x.begin() + 1, x.end());
    NTT<T>::fft(x);
    for (int i = 0; i < ((int) x.size() >> 1); i++) {
      b[i + ((int) x.size() >> 1)] = -x[i];
    }
  }
  b.resize(n);
  return b;
}
template<typename T>
std::vector<Modular<T>> inverse_old(std::vector<Modular<T>> a) {
  assert(!a.empty());
  int n = (int) a.size();
  if (n == 1) {
    return {1 / a[0]};
  }
  int m = (n + 1) >> 1;
  std::vector<Modular<T>> b =
      inverse(std::vector<Modular<T>>(a.begin(), a.begin() + m));
  int need = n << 1;
  int nbase = 0;
  while ((1 << nbase) < need) {
    ++nbase;
  }
  NTT<T>::ensure_base(nbase);
  int size = 1 << nbase;
  a.resize(size);
  b.resize(size);
  NTT<T>::fft(a);
  NTT<T>::fft(b);
  Modular<T> inv = 1 / static_cast<Modular<T>>(size);
  for (int i = 0; i < size; ++i) {
    a[i] = (2 - a[i] * b[i]) * b[i] * inv;
  }
  reverse(a.begin() + 1, a.end());
  NTT<T>::fft(a);
  a.resize(n);
  return a;
}
template<typename T>
std::vector<Modular<T>> operator*(const std::vector<Modular<T>> &a,
                                  const std::vector<Modular<T>> &b) {
  if (a.empty() || b.empty()) {
    return {};
  }
  if (std::min(a.size(), b.size()) < 150) {
    std::vector<Modular<T>> c(a.size() + b.size() - 1, 0);
    for (int i = 0; i < (int) a.size(); i++) {
      for (int j = 0; j < (int) b.size(); j++) {
        c[i + j] += a[i] * b[j];
      }
    }
    return c;
  }
  return NTT<T>::multiply(a, b);
}
template<typename T>
std::vector<Modular<T>> &operator*=(std::vector<Modular<T>> &a,
                                    const std::vector<Modular<T>> &b) {
  return a = a * b;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_NTT_HPP_
