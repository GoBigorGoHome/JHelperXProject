//
// Created by zjsdu on 5/28/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_ALIAS_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_ALIAS_HPP_
#include <string>
#include <cassert>
#include <queue>
#include <io.hpp>
using ll = long long;
using ull = unsigned long long;
using vl = std::vector<ll>;
using vb = std::vector<bool>;
using vi = std::vector<int>;
using vs = std::vector<std::string>;
using pii = std::pair<int, int>;
using pli = std::pair<ll, int>;
using pil = std::pair<int, ll>;
using pll = std::pair<ll, ll>;
using vii = std::vector<pii>;
template<typename T, typename U = std::less<T>>
using pq = std::priority_queue<T, std::vector<T>, U>;
template<typename... Ts> using vt = std::vector<std::tuple<Ts...>>;
template<typename T> class vv : public std::vector<std::vector<T>> {
  size_t n, m = 0;

 public:
  vv(size_t n, size_t m, T init = T{})
      : std::vector<std::vector<T>>(n, std::vector<T>(m, init)), n(n), m(m) {}
  using std::vector<std::vector<T>>::vector;
  void fill(T val) {
    assert(m > 0);
    for (auto &row : *this)
      std::fill(row.begin(), row.end(), val);
  }
  vv<T> transpose() const {
    assert(m > 0);
    vv<T> ret(m, n);
    for (int i = 0; i < m; ++i)
      for (int j = 0; j < n; ++j)
        ret[i][j] = (*this)[j][i];
    return ret;
  }
};
#define TOKENPASTE(x, y) x##y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define rng3(i, a, b)                                                          \
  for (std::common_type<decltype(a), decltype(b)>::type                        \
           i = a,                                                              \
           TOKENPASTE2(r_end_, __LINE__) = b;                                  \
       i < TOKENPASTE2(r_end_, __LINE__); ++i)
#define rng2(i, n) rng3(i, 0, n)
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define rng(...) GET_MACRO(__VA_ARGS__, rng3, rng2)(__VA_ARGS__)
#define up(i, a, b)                                                            \
  for (std::common_type<decltype(a), decltype(b)>::type                        \
           i = a,                                                              \
           TOKENPASTE2(r_end_, __LINE__) = b;                                  \
       i <= TOKENPASTE2(r_end_, __LINE__); ++i)
#define down(i, b, a)                                                          \
  for (std::common_type<decltype(a), decltype(b)>::type                        \
           i = b,                                                              \
           TOKENPASTE2(r_end_, __LINE__) = a;                                  \
       i >= TOKENPASTE2(r_end_, __LINE__); --i)
#define rep(n)                                                                 \
  for (auto TOKENPASTE2(_iter_, __LINE__) = n;                                 \
       TOKENPASTE2(_iter_, __LINE__) > 0; --TOKENPASTE2(_iter_, __LINE__))
#define FOR(x, cont) for (const auto &x : cont)
#define For(x, cont) for (auto &x : cont)
#define ALL(x) begin(x), end(x)
// hat off to 300iq
#define RALL(x) rbegin(x), rend(x)
#define pb push_back
#define mp make_pair
#define eb emplace_back
#define ep emplace
#define SZ(x) (int) (x).size()
#define rp(...) return pl(__VA_ARGS__)
#define rpn(...) return pn(__VA_ARGS__)
#define adv(i, n)                                                              \
  for (auto TOKENPASTE2(_n_, __LINE__) = n; i < TOKENPASTE2(_n_, __LINE__); ++i)
#define INT(...)                                                               \
  int __VA_ARGS__;                                                             \
  scan(__VA_ARGS__)
#define LL(...)                                                                \
  long long __VA_ARGS__;                                                       \
  scan(__VA_ARGS__)
#define STR(...)                                                               \
  std::string __VA_ARGS__;                                                     \
  scan(__VA_ARGS__)
#define rd(type, name, ...)                                                    \
  type name(__VA_ARGS__);                                                      \
  scan(name)
#define NL std::cout << '\n'
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_ALIAS_HPP_
