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
template<typename T> class vv {
  std::vector<std::vector<T>> data;

 public:
  explicit vv(int n) {
    assert(n > 0);
    data.assign(n, std::vector<T>{});
  }
  vv(int n, int m, T init = T{}) {
    assert(n > 0 && m > 0);
    data.assign(n, std::vector<T>(m, init));
  }
  std::vector<T> &operator[](size_t i) {
    assert(i < data.size());
    return data[i];
  }
  void fill(T val) {
    for (auto &row : data)
      std::fill(row.begin(), row.end(), val);
  }
  friend std::istream &operator>>(std::istream &in, vv<T> &val) {
    return io::operator>>(in, val.data);
  }
  friend std::ostream &operator<<(std::ostream &out, const vv<T> &val) {
    return io::operator<<(out, val.data);
  }
  friend auto begin(vv<T> &val) { return val.data.begin(); }
  friend auto end(vv<T> &val) { return val.data.end(); }
  friend auto rbegin(vv<T> &val) { return val.data.rbegin(); }
  friend auto rend(vv<T> &val) { return val.data.rend(); }
};

#define rng3(i, a, b)                                                          \
  for (std::common_type<decltype(a), decltype(b)>::type i = a; i < b; ++i)
#define rng2(i, n) rng3(i, 0, n)
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define rng(...) GET_MACRO(__VA_ARGS__, rng3, rng2)(__VA_ARGS__)
#define up(i, a, b)                                                            \
  for (std::common_type<decltype(a), decltype(b)>::type i = a; i <= b; ++i)
#define down(i, b, a)                                                          \
  for (std::common_type<decltype(a), decltype(b)>::type i = b; i >= a; i--)
#define TOKENPASTE(x, y) x##y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
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
#define UNIQ(vec)                                                              \
  sort(ALL(vec)), (vec).erase(std::unique(ALL(vec)), std::end(vec))
#define LB(cont, x) int(std::lower_bound(ALL(cont), x) - std::begin(cont))
#define UB(cont, x) int(std::upper_bound(ALL(cont), x) - std::begin(cont))
#define rp(...) return println(__VA_ARGS__)
#define adv(i, n)                                                              \
  for (auto TOKENPASTE2(_n_, __LINE__) = n; i < TOKENPASTE2(_n_, __LINE__); ++i)
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_ALIAS_HPP_
