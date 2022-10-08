//
// Created by zjsdu on 9/5/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GEOMETRY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GEOMETRY_HPP_
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>

// EPS不宜过小。
const double EPS = 1e-9;

int sign(double a) {
  return a <= -EPS ? -1 : a >= EPS ? 1 : 0;
}

bool fequal(double a, double b) {
  return abs(a - b) < EPS;
}

bool fless(double a, double b) {
  return a + EPS < b;
}

bool fleq(double a, double b) {
  return a < b + EPS;
}

struct point {
  double x;
  double y;
};

using points = std::vector<point>;
using vec = point;

double dot(vec a, vec b) {
  return a.x * b.x + a.y * b.y;
};

double cross(vec a, vec b) {
  return a.x * b.y - a.y * b.x;
}

double triangle_area(point a, point b, point c) {
  return abs(cross(a, b) + cross(b, c) + cross(c, a)) / 2;
}

std::istream &operator>>(std::istream &in, point &p) {
  in >> p.x >> p.y;
  return in;
}

std::ostream &operator<<(std::ostream &out, const point &p) {
  out << p.x << ' ' << p.y;
  return out;
}

double angle(point p) {
  return std::atan2(p.y, p.x);
}

point polar(double rho, double theta) {
  return {rho * cos(theta), rho * sin(theta)};
}

point operator+(point a, point b) {
  return {a.x + b.x, a.y + b.y};
}

point operator-(point a, point b) {
  return {a.x - b.x, a.y - b.y};
}

point operator*(double a, point p) {
  return {a * p.x, a * p.y};
}

point operator*(point p, double a) {
  return {a * p.x, a * p.y};
}

point operator/(point p, double a) {
  assert(a != 0);
  return {p.x / a, p.y / a};
}

bool operator==(point a, point b) {
  return fequal(a.x, b.x) and fequal(a.y, b.y);
}

bool operator!=(point a, point b) {
  return not(a == b);
}

double len(point p) {
  return std::sqrt(p.x * p.x + p.y * p.y);
}

double len(point p, point q) {
  return len(q - p);
}

double len2(point p) {
  return p.x * p.x + p.y * p.y;
}

// linear interpolation
point lerp(point a, point b, double t) {
  return a + t * (b - a);
}

/// @return 向量a方向上的单位向量
vec normalize(vec a) {
  assert(a.x != 0 or a.y != 0);
  return a / len(a);
}

struct circle {
  point c;
  double r;
  double &x = c.x;
  double &y = c.y;
};

points circle_intersection(const circle &c1, const circle &c2) {
  assert(c1.c != c2.c);
  double d = len(c2.c - c1.c);
  double r = c1.r + c2.r;
  if (fless(r, d) or fless(d, abs(c1.r - c2.r)))
    return {};
  double a = angle(c2.c - c1.c);
  double b = acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));
  point p = c1.c + polar(c1.r, a + b);
  point q = c1.c + polar(c1.r, a - b);
  if (p == q)
    return {p};
  return {p, q};
}

// 点 p 到直线 p1, p2 的距离
double distance(point p, point p1, point p2) {
  vec b = p2 - p1;
  vec a = p - p1;
  double ab = dot(a, b);
  return sqrt(dot(b, b) - ab * ab / (dot(a, a)));
}

// 两向量的角平分线
/// @pre 向量 a, b 不共线
vec bisector(vec a, vec b) {
  return normalize(len(b) * a + len(a) * b);
}

double sin(vec a, vec b) {
  return abs(dot(a, b)) / (len(a) * len(b));
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GEOMETRY_HPP_
