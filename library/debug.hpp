#include <bitset>
#include <iostream>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <sstream>
#ifdef LOCAL

#endif

namespace is_iterable_impl {
using std::begin, std::end;
template<typename T>
using check_specs = std::enable_if_t<
    std::is_same_v<decltype(begin(std::declval<T &>())),// has begin()
                   decltype(end(std::declval<T &>()))   // has end()
                   >>;// ... begin() and end() are the same type ...
/*
decltype(*begin(std::declval<T &>())) // ... which can be dereferenced
*/

template<class T, class = void> struct is_iterable : std::false_type {};
// specialization
template<class T> struct is_iterable<T, check_specs<T>> : std::true_type {};
}// namespace is_iterable_impl

template<class T> using is_iterable = is_iterable_impl::is_iterable<T>;

template<class T> constexpr bool is_iterable_v = is_iterable<T>::value;

namespace debug {
template<typename... Args>
std::ostream &operator<<(std::ostream &os, std::tuple<Args...> const &t);
std::ostream &operator<<(std::ostream &os, bool b) {
  return os << (b ? "true" : "false");
}
std::ostream &operator<<(std::ostream &os, const std::string &s) {
  using std::operator<<;
  return os << '"' << s << '"';
}
std::ostream &operator<<(std::ostream &os, const char *s) {
  using std::operator<<;
  return os << '"' << s << '"';
}
template<typename A, typename B>
std::ostream &operator<<(std::ostream &os, const std::pair<A, B> &p) {
  os << '(' << p.first;
  os << ',' << ' ' << p.second;
  return os << ')';
}
template<typename A, typename B = std::enable_if_t<is_iterable_v<A>>>
std::ostream &operator<<(std::ostream &os, const A &v) {
  bool first = true;
  os << '{';
  for (const auto &x : v) {
    if (!first) {
      os << ',' << ' ';
    }
    first = false;
    os << x;
  }
  return os << '}';
}

template<typename... Ts>
std::ostream &operator<<(std::ostream &out, const std::tuple<Ts...> &t) {
  bool first = true;
  out << '(';
  std::apply(
      [&out, &first](auto &... args) {
        ((std::operator<<(out, (first ? first = false, "" : ", ")) << args),
         ...);
      },
      t);
  out << ')';
  return out;
}
}// namespace debug

extern std::ostringstream debug_stream;
void debug_out() {
  debug_stream << std::endl;
}

template<typename Head, typename... Tail>
void debug_out(const Head &H, const Tail &... T) {
  debug_stream << " ";
  using namespace debug;
  debug_stream << H;
  debug_out(T...);
}

#define debug(...)                                                             \
  debug_stream << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
