//
// Created by zjsdu on 9/27/2021.
// A 2D geometry library that considers only integral points.

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_IGEOMETRY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_IGEOMETRY_HPP_
#include <iostream>
#include <numeric>
#include <vector>
#include <cassert>

typedef long long ll;

struct point {
  ll x, y;
};

typedef std::vector<point> points;

bool operator==(const point &a, const point &b) {
  return a.x == b.x and a.y == b.y;
}

bool operator!=(const point &a, const point &b) {
  return not(a == b);
}

bool operator<(const point &a, const point &b) {
  return a.x == b.x ? a.y < b.y : a.x < b.x;
}

bool operator>(const point &a, const point &b) {
  return b < a;
}

bool operator<=(const point &a, const point &b) {
  return not(b < a);
}

bool operator>=(const point &a, const point &b) {
  return not(a < b);
}

std::istream &operator>>(std::istream &in, point &p) {
  in >> p.x >> p.y;
  return in;
}

std::ostream &operator<<(std::ostream &out, const point &p) {
  out << "(" << p.x << ", " << p.y << ")";
  return out;
}

typedef point vec;

ll dot(vec a, vec b) {
  return a.x * b.x + a.y * b.y;
}

ll cross(vec a, vec b) {
  return a.x * b.y - a.y * b.x;
}

vec operator+(vec a, vec b) {
  return {a.x + b.x, a.y + b.y};
}

vec &operator+=(vec &a, vec b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

vec operator-(vec a, vec b) {
  return {a.x - b.x, a.y - b.y};
}

vec &operator-=(vec &a, vec b) {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

vec operator*(ll a, vec v) {
  return {a * v.x, a * v.y};
}

vec operator*(vec v, ll a) {
  return a * v;
}

vec &operator*=(vec &a, ll b) {
  a.x *= b;
  a.y *= b;
  return a;
}

vec operator/(vec v, ll a) {
  assert(a != 0);
  assert(v.x % a == 0);
  assert(v.y % a == 0);
  return {v.x / a, v.y / a};
}

vec &operator/=(vec &v, ll a) {
  assert(a != 0);
  assert(v.x % a == 0);
  assert(v.y % a == 0);
  v.x /= a;
  v.y /= a;
  return v;
}

vec normalize(vec v) {
  if (v.x == 0) {
    if (v.y == 0)
      return v;
    return {0, 1};
  }
  if (v.x < 0) {
    v.x *= -1;
    v.y *= -1;
  }
  ll g = std::gcd(v.x, v.y);
  v.x /= g;
  v.y /= g;
  return v;
}

vec get_y_intercept(point p, vec direction) {
  if (direction.x == 0) {
    return {p.x, 0};
  }
  if (direction.x < 0) {
    direction.x *= -1;
    direction.y *= -1;
  }
  ll fz = p.y * direction.x - p.x * direction.y;
  ll fm = direction.x;
  ll g = std::gcd(fz, fm);
  return {fz / g, fm / g};
};

int quadrant(const point& p) {
  assert(p.x != 0 or p.y != 0);
  if (p.x > 0 and p.y >= 0)
    return 1;
  if (p.y > 0 and p.x <= 0)
    return 1;
  if (p.x < 0 and p.y <= 0)
    return 3;
  return 4;
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_IGEOMETRY_HPP_
