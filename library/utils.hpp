#ifndef CP_UTILS
#define CP_UTILS
#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstring>
#include <map>
#include <numeric>
#include <set>
#include <random>
#include <chrono>
#include <alias.hpp>
#include <type_traits.hpp>

inline void Yn(bool p) {
  std::cout << (p ? "Yes\n" : "No\n");
}
inline void YN(bool p) {
  std::cout << (p ? "YES\n" : "NO\n");
}
inline void yn(bool p) {
  std::cout << (p ? "yes\n" : "no\n");
}
template<typename Container> Container inc(Container &&c) {
  for (auto &e : c)
    ++e;
  return std::forward<Container>(c);
}
template<typename Container> Container dec(Container &&c) {
  for (auto &e : c)
    --e;
  return std::forward<Container>(c);
}

#if __cplusplus >= 201703L
template<typename A, typename B, typename... C>
bool chkmin(A& a, const B& b, const C&... c) {
  if (B res = std::min<B>({b, c...}); res < a) {
    a = res;
    return true;
  }
  return false;
}

template<typename A, typename B, typename... C>
bool chkmax(A& a, const B& b, const C&... c) {
  if (B res = std::max<B>({b, c...}); res > a) {
    a = res;
    return true;
  }
  return false;
}
#endif

template<typename Container> Container reverse(Container &&c) {
  std::reverse(std::begin(c), std::end(c));
  return std::forward<Container>(c);
}

template<typename Sequence> Sequence rev(Sequence a) {
  std::reverse(std::begin(a), std::end(a));
  return a;
}

template<typename Sequence> Sequence uniq(Sequence &&s) {
  std::sort(std::begin(s), std::end(s));
  s.erase(std::unique(std::begin(s), std::end(s)), std::end(s));
  return std::forward<Sequence>(s);
}

template<typename Container> auto max(const Container &c) {
  assert(c.size() > 0);
  return *std::max_element(std::begin(c), std::end(c));
}

template<typename Container> auto min(const Container &c) {
  assert(c.size() > 0);
  return *std::min_element(std::begin(c), std::end(c));
}

template<typename Array> int maxi(const Array &a) {
  assert(a.size() > 0);
  return int(std::max_element(std::begin(a), std::end(a)) - std::begin(a));
}

template<typename Array> int mini(const Array &a) {
  assert(a.size() > 0);
  return int(std::min_element(std::begin(a), std::end(a)) - std::begin(a));
}

template<typename Array, typename Value> auto lb(const Array &a, Value v) {
  return std::lower_bound(std::begin(a), std::end(a), v);
}

template<typename Array, typename Value> auto ub(const Array &a, Value v) {
  return std::upper_bound(std::begin(a), std::end(a), v);
}

template<typename Array, typename Value> int lbi(const Array &a, Value v) {
  return int(lb(a, v) - std::begin(a));
}

template<typename Iter, typename Value> int lbi(Iter beg, int count, Value v) {
  assert(count > 0);
  return int(std::lower_bound(beg, beg + count, v) - beg);
}

template<typename Iter, typename Value> int ubi(Iter beg, int count, Value v) {
  assert(count > 0);
  return int(std::upper_bound(beg, beg + count, v) - beg);
}

template<typename Array, typename Value> int ubi(const Array &a, Value v) {
  return int(ub(a, v) - std::begin(a));
}

#if __cplusplus >=201703L
template<typename Container>
Container iota(Container &&c, value_type_of<Container> v) {
  std::iota(std::begin(c), std::end(c), v);
  return std::forward<Container>(c);
}
#endif

template<typename Container, typename Compare = void *>
Container sort(Container &&c, Compare comp = nullptr) {
  if (std::is_same<Compare, void *>::value)
    std::sort(std::begin(c), std::end(c));
  else
    std::sort(std::begin(c), std::end(c), comp);
  return std::forward<Container>(c);
}

template<typename T> struct reversion_wrapper { T &iterable; };
template<typename T> auto begin(reversion_wrapper<T> w) {
  using std::rbegin;
  return rbegin(w.iterable);
}
template<typename T> auto end(reversion_wrapper<T> w) {
  using std::rend;
  return rend(w.iterable);
}
template<typename T> reversion_wrapper<T> Reverse(T &&iterable) {
  return {std::forward<T>(iterable)};
}

/// @return nearest integer not less than the quotient x/y.
template<typename T, typename U> T qceil(T x, U y) {
  assert(y > 0);
  T q = x / y;
  return q + (q * y < x);
}

/// @return nearest integer not greater than the quotient x/y.
template<typename T, typename U> T qfloor(T x, U y) {
  assert(y > 0);
  T q = x / y;
  return q - (q * y > x);
}

/// @return nearest multiple of y not less than x.
template<typename T, typename U> T mceil(T x, U y) {
  assert(y > 0);
  return qceil(x, y) * y;
}

/// @return nearest multiple of y not greater than x.
template<typename T, typename U> T mfloor(T x, U y) {
  assert(y > 0);
  return qfloor(x, y) * y;
}

#if __cplusplus >= 201703L
template<class...> struct typelist {};

template<class T, class... Ts>
constexpr bool any_same = (std::is_same<T, Ts>::value || ...);

template<class F> struct y_combinator {
  template<class... TLs> struct ref {
    y_combinator &self;
    template<class... Args> decltype(auto) operator()(Args &&...args) const {
      using G = std::conditional_t<any_same<typelist<Args...>, TLs...>,
                                   ref<TLs...>, ref<TLs..., typelist<Args...>>>;
      return self.f(G{self}, std::forward<Args>(args)...);
    }
  };
  F f;
  template<class... Args> decltype(auto) operator()(Args &&...args) {
    return ref<>{*this}(std::forward<Args>(args)...);
  }
};
template<class F> y_combinator(F) -> y_combinator<F>;
#endif

template<typename T> constexpr T INF = std::numeric_limits<T>::max() / 2;

/// @brief Usage: acc\<type_of_sum\>(array)
template<typename T, typename U> T acc(const U& array) {
  return std::accumulate(std::begin(array), std::end(array), T(0));
}

template <typename T> T acc(const std::vector<T>& array) {
  return std::accumulate(array.begin(), array.end(), T(0));
}

#ifndef debug
#define debug(...)
#endif
#ifndef show
#define show(...)
#endif
using namespace io;
using namespace std;
#endif// CP_UTILS
