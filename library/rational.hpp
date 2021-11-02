// Created by zjsdu on 8/6/2020.
#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_RATIONAL_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_RATIONAL_HPP_

//  (C) Copyright Paul Moore 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/libs/rational for documentation.

#include <stdexcept>// for std::domain_error
#include <limits>   // for std::numeric_limits
#include <numeric>
#include <sstream>
#include <iomanip>
#include <cassert>

class bad_rational : public std::domain_error {
 public:
  explicit bad_rational()
      : std::domain_error("bad rational: zero denominator") {}
  explicit bad_rational(char const *what) : std::domain_error(what) {}
};

template<typename IntType> class rational {
  // Class-wide pre-conditions
  static_assert(std::numeric_limits<IntType>::is_specialized);

 public:
  // Component type
  using int_type = IntType;

  // Constructors
  constexpr rational() : num(0), den(1) {}

  template<class T> constexpr rational(const T &n) : num{n}, den(1) {}

  template<class T, class U>
  constexpr rational(const T &n, const U &d) : num{n}, den{d} {
    normalize();
  }

  // Copy constructors
  template<typename NewType>
  constexpr explicit rational(rational<NewType> const &r)
      : num{r.numerator()}, den{r.denominator()} {}

  // Add assignment from IntType
  template<class T> constexpr rational &operator=(const T &n) {
    return assign(IntType{n}, static_cast<IntType>(1));
  }

  // Assign in place
  template<class T, class U> constexpr rational assign(const T &n, const U &d) {
    return *this = rational<IntType>(IntType{n}, IntType{d});
  }

  // Access to representation
  constexpr const IntType &numerator() const { return num; }
  constexpr const IntType &denominator() const { return den; }

  // Arithmetic assignment operators
  constexpr rational &operator+=(const rational &r);
  constexpr rational &operator-=(const rational &r);
  constexpr rational &operator*=(const rational &r);
  constexpr rational &operator/=(const rational &r);

  template<class T> constexpr rational &operator+=(const T &i) {
    num += IntType{i} * den;
    return *this;
  }
  template<class T> constexpr rational &operator-=(const T &i) {
    num -= IntType{i} * den;
    return *this;
  }
  template<class T> constexpr rational &operator*=(const T &i) {
    // Avoid overflow and preserve normalization
    IntType gcd = std::gcd(IntType{i}, den);
    num *= i / gcd;
    den /= gcd;
    return *this;
  }
  template<class T> constexpr rational &operator/=(const T &i) {
    // Avoid repeated construction
    IntType const zero(0);

    if (IntType{i} == zero)
      throw bad_rational();
    if (num == zero)
      return *this;

    // Avoid overflow and preserve normalization
    IntType const gcd = std::gcd(num, static_cast<IntType>(i));
    num /= gcd;
    den *= i / gcd;

    if (den < zero) {
      num = -num;
      den = -den;
    }

    return *this;
  }

  // Operator not
  constexpr bool operator!() const { return !num; }

  // Boolean conversion
  constexpr explicit operator bool() const { return num; }

  // Comparison operators
  constexpr bool operator<(const rational &r) const;
  constexpr bool operator>(const rational &r) const { return r < *this; }
  constexpr bool operator==(const rational &r) const;

  template<class T> constexpr bool operator<(const T &i) const {
    // Avoid repeated construction
    int_type const zero(0);

    // Break value into mixed-fraction form, w/ always-nonnegative remainder
    assert(this->den > zero);
    int_type q = this->num / this->den;
    int_type r = this->num % this->den;
    while (r < zero) {
      r += this->den;
      --q;
    }

    // Compare with just the quotient, since the remainder always bumps the
    // value up.  [Since q = floor(n/d), and if n/d < i then q < i, if n/d == i
    // then q == i, if n/d == i + r/d then q == i, and if n/d >= i + 1 then
    // q >= i + 1 > i; therefore n/d < i iff q < i.]
    return q < i;
  }
  template<class T> constexpr bool operator>(const T &i) const {
    return operator==(i) ? false : !operator<(i);
  }
  template<class T> constexpr bool operator==(const T &i) const {
    return (den == IntType(1)) && (num == i);
  }

 private:
  // Implementation - numerator and denominator (normalized).
  // Other possibilities - separate whole-part, or sign, fields?
  IntType num;
  IntType den;

  // Representation note: Fractions are kept in normalized form at all
  // times. normalized form is defined as gcd(num,den) == 1 and den > 0.
  // In particular, note that the implementation of abs() below relies
  // on den always being positive.
  constexpr bool test_invariant() const;
  constexpr void normalize();
};

// Unary plus and minus
template<typename IntType>
constexpr rational<IntType> operator+(const rational<IntType> &r) {
  return r;
}

template<typename IntType>
constexpr rational<IntType> operator-(const rational<IntType> &r) {
  return rational<IntType>(static_cast<IntType>(-r.numerator()),
                           r.denominator());
}

// Arithmetic assignment operators
template<typename IntType>
constexpr rational<IntType> &
rational<IntType>::operator+=(const rational<IntType> &r) {
  // This calculation avoids overflow, and minimises the number of expensive
  // calculations. Thanks to Nickolay Mladenov for this algorithm.
  //
  // Proof:
  // We have to compute a/b + c/d, where gcd(a,b)=1 and gcd(b,c)=1.
  // Let g = gcd(b,d), and b = b1*g, d=d1*g. Then gcd(b1,d1)=1
  //
  // The result is (a*d1 + c*b1) / (b1*d1*g).
  // Now we have to normalize this ratio.
  // Let's assume h | gcd((a*d1 + c*b1), (b1*d1*g)), and h > 1
  // If h | b1 then gcd(h,d1)=1 and hence h|(a*d1+c*b1) => h|a.
  // But since gcd(a,b1)=1 we have h=1.
  // Similarly h|d1 leads to h=1.
  // So we have that h | gcd((a*d1 + c*b1) , (b1*d1*g)) => h|g
  // Finally we have gcd((a*d1 + c*b1), (b1*d1*g)) = gcd((a*d1 + c*b1), g)
  // Which proves that instead of normalizing the result, it is better to
  // divide num and den by gcd((a*d1 + c*b1), g)

  // Protect against self-modification
  IntType r_num = r.num;
  IntType r_den = r.den;

  IntType g = std::gcd(den, r_den);
  den /= g;// = b1 from the calculations above
  num = num * (r_den / g) + r_num * den;
  g = std::gcd(num, g);
  num /= g;
  den *= r_den / g;

  return *this;
}

template<typename IntType>
constexpr rational<IntType> &
rational<IntType>::operator-=(const rational<IntType> &r) {
  // Protect against self-modification
  IntType r_num = r.num;
  IntType r_den = r.den;

  // This calculation avoids overflow, and minimises the number of expensive
  // calculations. It corresponds exactly to the += case above
  IntType g = std::gcd(den, r_den);
  den /= g;
  num = num * (r_den / g) - r_num * den;
  g = std::gcd(num, g);
  num /= g;
  den *= r_den / g;

  return *this;
}

template<typename IntType>
constexpr rational<IntType> &
rational<IntType>::operator*=(const rational<IntType> &r) {
  // Protect against self-modification
  IntType r_num = r.num;
  IntType r_den = r.den;

  // Avoid overflow and preserve normalization
  IntType gcd1 = std::gcd(num, r_den);
  IntType gcd2 = std::gcd(r_num, den);
  num = (num / gcd1) * (r_num / gcd2);
  den = (den / gcd2) * (r_den / gcd1);
  return *this;
}

template<typename IntType>
constexpr rational<IntType> &
rational<IntType>::operator/=(const rational<IntType> &r) {
  // Protect against self-modification
  IntType r_num = r.num;
  IntType r_den = r.den;

  // Avoid repeated construction
  IntType zero(0);

  // Trap division by zero
  if (r_num == zero)
    throw bad_rational();
  if (num == zero)
    return *this;

  // Avoid overflow and preserve normalization
  IntType gcd1 = std::gcd(num, r_num);
  IntType gcd2 = std::gcd(r_den, den);
  num = (num / gcd1) * (r_den / gcd2);
  den = (den / gcd2) * (r_num / gcd1);

  if (den < zero) {
    num = -num;
    den = -den;
  }
  return *this;
}
//
// Non-member operators:
// There are three possible cases for each operator:
// 1) rational op rational.
// 2) rational op integer
// 3) integer op rational
//
template<class IntType, class Arg>
constexpr rational<IntType> operator+(const rational<IntType> &a,
                                      const Arg &b) {
  rational<IntType> t(a);
  return t += b;
}

template<class Arg, class IntType>
constexpr typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>,
                                    rational<IntType>>
operator+(const Arg &b, const rational<IntType> &a) {
  rational<IntType> t(a);
  return t += b;
}

template<class IntType, class Arg>
constexpr rational<IntType> operator-(const rational<IntType> &a,
                                      const Arg &b) {
  rational<IntType> t(a);
  return t -= b;
}

template<class Arg, class IntType>
constexpr typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>,
                                    rational<IntType>>
operator-(const Arg &b, const rational<IntType> &a) {
  rational<IntType> t(a);
  return -(t -= b);
}

template<class IntType, class Arg>
constexpr rational<IntType> operator*(const rational<IntType> &a,
                                      const Arg &b) {
  rational<IntType> t(a);
  return t *= b;
}

template<class Arg, class IntType>
constexpr typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>,
                                    rational<IntType>>
operator*(const Arg &b, const rational<IntType> &a) {
  rational<IntType> t(a);
  return t *= b;
}

template<class IntType, class Arg>
constexpr rational<IntType> operator/(const rational<IntType> &a,
                                      const Arg &b) {
  rational<IntType> t(a);
  return t /= b;
}

template<class Arg, class IntType>
constexpr typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>,
                                    rational<IntType>>
operator/(const Arg &b, const rational<IntType> &a) {
  rational<IntType> t(b);
  return t /= a;
}

template<class IntType, class Arg>
constexpr bool operator<=(const rational<IntType> &a, const Arg &b) {
  return !(a > b);
}

template<class Arg, class IntType>
constexpr
    typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>, bool>
    operator<=(const Arg &b, const rational<IntType> &a) {
  return a >= b;
}

template<class IntType, class Arg>
constexpr bool operator>=(const rational<IntType> &a, const Arg &b) {
  return !(a < b);
}

template<class Arg, class IntType>
constexpr
    typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>, bool>
    operator>=(const Arg &b, const rational<IntType> &a) {
  return a <= b;
}

template<class IntType, class Arg>
constexpr bool operator!=(const rational<IntType> &a, const Arg &b) {
  return !(a == b);
}

template<class Arg, class IntType>
constexpr
    typename std::enable_if_t<not std::is_same_v<rational<IntType>, Arg>, bool>
    operator!=(const Arg &b, const rational<IntType> &a) {
  return !(b == a);
}

template<class Arg, class IntType>
constexpr bool operator<(const Arg &b, const rational<IntType> &a) {
  return a > b;
}

template<class Arg, class IntType>
constexpr bool operator>(const Arg &b, const rational<IntType> &a) {
  return a < b;
}

template<class Arg, class IntType>
constexpr bool operator==(const Arg &b, const rational<IntType> &a) {
  return a == b;
}

// Comparison operators
template<typename IntType>
constexpr bool rational<IntType>::operator<(const rational<IntType> &r) const {
  // Avoid repeated construction
  int_type const zero(0);

  // This should really be a class-wide invariant.  The reason for these
  // checks is that for 2's complement systems, INT_MIN has no corresponding
  // positive, so negating it during normalization keeps it INT_MIN, which
  // is bad for later calculations that assume a positive denominator.
  assert(this->den >= zero);
  assert(r.den >= zero);

  if (this->den == 0) {
    if (r.den == 0)
      return this->num < r.num;
    return this->num < 0;
  }
  if (r.den == 0) {
    return r.num > 0;
  }

  // Determine relative order_ by expanding each value to its simple continued
  // fraction representation using the Euclidean GCD algorithm.
  struct {
    int_type n, d, q, r;
  } ts = {this->num, this->den, static_cast<int_type>(this->num / this->den),
          static_cast<int_type>(this->num % this->den)},
    rs = {r.num, r.den, static_cast<int_type>(r.num / r.den),
          static_cast<int_type>(r.num % r.den)};
  unsigned reverse = 0u;

  // Normalize negative moduli by repeatedly adding the (positive) denominator
  // and decrementing the quotient.  Later cycles should have all positive
  // values, so this only has to be done for the first cycle.  (The rules of
  // C++ require a nonnegative quotient & remainder for a nonnegative dividend
  // & positive divisor.)
  while (ts.r < zero) {
    ts.r += ts.d;
    --ts.q;
  }
  while (rs.r < zero) {
    rs.r += rs.d;
    --rs.q;
  }

  // Loop through and compare each variable's continued-fraction components
  for (;;) {
    // The quotients of the current cycle are the continued-fraction
    // components.  Comparing two c.f. is comparing their sequences,
    // stopping at the first difference.
    if (ts.q != rs.q) {
      // Since reciprocation changes the relative order_ of two variables,
      // and c.f. use reciprocals, the less/greater-than test reverses
      // after each index.  (Start w/ non-reversed @ whole-number place.)
      return reverse ? ts.q > rs.q : ts.q < rs.q;
    }

    // Prepare the next cycle
    reverse ^= 1u;

    if ((ts.r == zero) || (rs.r == zero)) {
      // At least one variable's c.f. expansion has ended
      break;
    }

    ts.n = ts.d;
    ts.d = ts.r;
    ts.q = ts.n / ts.d;
    ts.r = ts.n % ts.d;
    rs.n = rs.d;
    rs.d = rs.r;
    rs.q = rs.n / rs.d;
    rs.r = rs.n % rs.d;
  }

  // Compare infinity-valued components for otherwise equal sequences
  if (ts.r == rs.r) {
    // Both remainders are zero, so the next (and subsequent) c.f.
    // components for both sequences are infinity.  Therefore, the sequences
    // and their corresponding values are equal.
    return false;
  } else {
    // Exactly one of the remainders is zero, so all following c.f.
    // components of that variable are infinity, while the other variable
    // has a finite next c.f. component.  So that other variable has the
    // lesser value (modulo the reversal flag!).
    return (ts.r != zero) != static_cast<bool>(reverse);
  }
}

template<typename IntType>
constexpr bool rational<IntType>::operator==(const rational<IntType> &r) const {
  return ((num == r.num) && (den == r.den));
}

// Invariant check
template<typename IntType>
constexpr bool rational<IntType>::test_invariant() const {
  return (this->den >= int_type(0))
      && (std::gcd(this->num, this->den) == int_type(1));
}

// Normalisation
template<typename IntType> constexpr void rational<IntType>::normalize() {
  // Avoid repeated construction
  IntType zero(0);

  if (den == zero && num == zero)
    throw bad_rational("bad rational: 0/0");

  if (den < -(std::numeric_limits<IntType>::max)()) {
    throw bad_rational("bad rational: non-zero singular denominator");
  }

  IntType g = std::gcd(num, den);

  num /= g;
  den /= g;

  // Ensure that the denominator is positive
  if (den < zero) {
    num = -num;
    den = -den;
  }

  assert(this->test_invariant());
}

// Output
template<typename IntType>
std::ostream &operator<<(std::ostream &os, const rational<IntType> &r) {
  // The slash directly precedes the denominator, which has no prefixes.
  std::ostringstream ss;

  ss.copyfmt(os);
  ss.tie(nullptr);
  ss.exceptions(std::ios::goodbit);
  ss.width(0);
  ss << std::noshowpos << std::noshowbase << '/' << r.denominator();

  // The numerator holds the showpos, internal, and showbase flags.
  std::string const tail = ss.str();
  std::streamsize const w =
      os.width() - static_cast<std::streamsize>(tail.size());

  ss.clear();
  ss.str("");
  ss.flags(os.flags());
  ss << std::setw(
      w < 0 || (os.flags() & std::ios::adjustfield) != std::ios::internal ? 0
                                                                          : w)
     << r.numerator();
  return os << ss.str() + tail;
}

// Type conversion
template<typename T, typename IntType>
constexpr T rational_cast(const rational<IntType> &src) {
  return static_cast<T>(src.numerator()) / static_cast<T>(src.denominator());
}

// Do not use any abs() defined on IntType - it isn't worth it, given the
// difficulties involved (Koenig lookup required, there may not *be* an abs()
// defined, etc etc).
template<typename IntType>
constexpr rational<IntType> abs(const rational<IntType> &r) {
  return r.numerator() >= IntType(0) ? r : -r;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_RATIONAL_HPP_
