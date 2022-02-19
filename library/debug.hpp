#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <bitset>
#include <ostream>
#include <iterator>
#include <tuple>
#include <sstream>
#include <type_traits.hpp>

namespace debug {
template<typename... Args>
std::ostream &operator<<(std::ostream &, std::tuple<Args...> const &);
std::ostream &operator<<(std::ostream &out, const std::string &s) {
  using std::operator<<;
  return out << '"' << s << '"';
}
std::ostream &operator<<(std::ostream &out, const char *s) {
  using std::operator<<;
  return out << '"' << s << '"';
}
template<typename A, typename B>
std::ostream &operator<<(std::ostream &out, const std::pair<A, B> &p) {
  out << '(' << p.first;
  out << ',' << ' ' << p.second;
  return out << ')';
}
template<typename Container,
         typename = std::enable_if_t<is_iterable_v<Container>>>
std::ostream &operator<<(std::ostream &out, const Container &container) {
  bool first = true;
  out << '{';
  for (const auto &element : container) {
    if (!first) {
      out << ',' << ' ';
    }
    first = false;
    out << element;
  }
  return out << '}';
}

template<typename... Ts>
std::ostream &operator<<(std::ostream &out, const std::tuple<Ts...> &t) {
  bool first = true;
  out << '(';
  std::apply(
      [&out, &first](const Ts &...args) {
        ((std::operator<<(out, (first ? first = false, "" : ", ")) << args),
         ...);
      },
      t);
  return out << ')';
}

template<typename... Args>
void debug_out(std::ostream &out, const Args &...args) {
  ((out << ' ' << args), ...);
  out << '\n';
}
}// namespace debug
#define show(...)                                                              \
  std::cerr << "[" << #__VA_ARGS__ << "]:",                                    \
      debug::debug_out(std::cerr, __VA_ARGS__)

#endif
