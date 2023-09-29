//
// Created by zjsdu on 9/5/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GEOMETRY_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GEOMETRY_HPP_
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>
#include <geodeb.h>
// EPS不宜过小。
const double EPS = 1e-9;

int sign(double a) {
  return a <= -EPS ? -1 : a >= EPS ? 1 : 0;
}

bool fequal(double a, double b) {
  return std::abs(a - b) < EPS;
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
  point operator/(double d) const { return {x / d, y / d}; }
  double len() const { return std::sqrt(x * x + y * y); }
  double len2() const { return x * x + y * y; }
  point unit() const { return *this / len(); }
  // 逆时针旋转90度
  point perp() const { return {-y, x}; }
  point normal() const { return perp().unit(); }
};

std::istream &operator>>(std::istream &in, point &p) {
  in >> p.x >> p.y;
  return in;
}

std::ostream &operator<<(std::ostream &out, const point &p) {
  out << p.x << ' ' << p.y;
  return out;
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

bool operator==(point a, point b) {
  return fequal(a.x, b.x) and fequal(a.y, b.y);
}

bool operator!=(point a, point b) {
  return not(a == b);
}

using points = std::vector<point>;
using vec = point;

double dot(vec a, vec b) {
  return a.x * b.x + a.y * b.y;
};

double cross(vec a, vec b) {
  return a.x * b.y - a.y * b.x;
}

// 三点共线
bool collinear(point a, point b, point c) {
  return fequal(cross(b - a, c - a), 0);
}

// 四点共线
bool collinear(point a, point b, point c, point d) {
  return collinear(a, b, c) && collinear(a, b, d);
}

bool parallel(vec a, vec b) {
  return fequal(cross(a, b), 0);
}

double angle(point p) {
  return std::atan2(p.y, p.x);
}

point polar(double rho, double theta) {
  return {rho * cos(theta), rho * sin(theta)};
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

point line_intersection(point a1, point b1, point a2, point b2) {
  double d = cross(b1 - a1, b2 - a2);
  assert(!fequal(d, 0));
  double p = cross(b1 - a2, b2 - a2);
  double q = cross(b2 - a2, a1 - a2);
  return (a1 * p + b1 * q) / d;
}

// 点p到直线ab的距离
double dist(point p, point a, point b) {
  return std::abs(cross(b - a, p - a)) / len(a, b);
}

// 点p到直线ab的垂足
point foot(point p, point a, point b) {
  double h = cross(a - p, b - p) / len(a, b);
  return p - h * (b - a).normal();
}

// 点p关于直线ab的对称点
point mirror(point p, point a, point b) {
  double h = cross(a - p, b - p) / len(a, b);
  return p - 2 * h * (b - a).normal();
}

// 线段
struct segment {
  point a, b;
  // 法向量
  vec normal() const { return {b.y - a.y, a.x - b.x}; }
};

// 圆
struct circle {
  point center;
  double r;
  circle(point c, double r) : center(c), r(r) {}
  // AB是直径
  circle(point A, point B) {
    center = (A + B) / 2;
    r = len(center, A);
  }

  circle(point A, point B, point C) {
    assert(!collinear(A, B, C));
    /* 三角形ABC的面积 = a*b*c/4R
     * a,b,c是三边长，R是外接圆半径*/
    r = len(A, B) * len(B, C) * len(A, C) / std::abs(cross(B - A, C - A)) / 2;
    vec b = C - A, c = B - A;
    center = A + (b * c.len2() - c * b.len2()).perp() / cross(b, c) / 2;
  }

  // 点在圆内
  bool contains(point p) const { return fleq(len(p, center), r); }
  // 判断圆和线段是否相交
  bool intersect(segment s) const {
    if (contains(s.a) || contains(s.b))
      return true;
    if (!fequal(r, dist(center, s.a, s.b)))
      return false;
    // 线段ab的法向量和ca,cb叉乘，看符号
    int u = sign(cross(s.a - center, s.normal()));
    int v = sign(cross(s.b - center, s.normal()));
    if (u == 0 || v == 0)
      return true;
    return u != v;
  }
};

points circle_intersection(const circle &c1, const circle &c2) {
  assert(c1.center != c2.center);
  double d = len(c2.center - c1.center);
  double r = c1.r + c2.r;
  if (fless(r, d) or fless(d, std::abs(c1.r - c2.r)))
    return {};
  double a = angle(c2.center - c1.center);
  double b = acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));
  point p = c1.center + polar(c1.r, a + b);
  point q = c1.center + polar(c1.r, a - b);
  if (p == q)
    return {p};
  return {p, q};
}

// 两向量的角平分线
/// @pre 向量 a, b 不共线
vec bisector(vec a, vec b) {
  return (len(b) * a + len(a) * b).unit();
}

double sin(vec a, vec b) {
  return std::abs(dot(a, b)) / (len(a) * len(b));
}

double triangle_area(point a, point b, point c) {
  return std::abs(cross(b - a, c - a)) / 2;
}

// 直线
struct line {
  point a;
  point b;
  line(point a, point b) : a(a), b(b) {}
  bool contains(point p) const { return collinear(a, b, p); }
  point unit() const { return (b - a).unit(); }
};

bool parallel(line A, line B) {
  return parallel(A.a - A.b, B.a - B.b);
}

// 点p到直线L的距离
double dist(point p, line L) {
  return dist(p, L.a, L.b);
}

// 点p关于直线L的对称点
point mirror(point p, line L) {
  return mirror(p, L.a, L.b);
}

// 点p在直线L上的垂足
point foot(point p, line L) {
  return foot(p, L.a, L.b);
}

point intersection(line A, line B) {
  return line_intersection(A.a, A.b, B.a, B.b);
}

std::vector<line> bisectors(line A, line B) {
  if (parallel(A, B)) {
    point c = foot(A.a, B);
    vec d = (c - A.a) / 2;
    return {{A.a + d, A.b + d}};
  }
  point p = intersection(A, B);
  vec dir = bisector(A.b - A.a, B.b - B.a);
  return {{p, p + dir}, {p, p + dir.perp()}};
}

// 由圆上两点和一条切线确定圆
std::vector<circle> get_circles_by_ppt(point a, point b, line T) {
  point m = (a + b) / 2;
  // 割线和切线平行
  //  GD_LAYER() << "Circle";
  if (parallel(b - a, T.b - T.a)) {
    if (collinear(a, T.a, T.b))
      return {};
    point ft = foot(m, T);
    //    GD_POINT(ft.x, ft.y);
    return {circle(a, b, ft)};
  }
  // 割线和切线不平行。
  line L(m, m + (b - a).perp());// L是一条直径
  // 用切割线定理求出切点，进而求出一条直径
  point p = line_intersection(a, b, T.a, T.b);// 切线和割线的交点
  if (fless(dot(p - a, p - b), 0))            // p在a,b之间
    return {};
  //  GD_LINE(T.a.x, T.a.y, T.b.x, T.b.y, "purple");
  //  GD_LINE(a.x, a.y, b.x, b.y, "pink");
  //
  //  GD_POINT(p.x, p.y);

  double l = std::sqrt(len(p, a) * len(p, b));
  assert(l >= 0);
  vec d = l * (T.b - T.a).unit();
  std::vector<circle> ans;
  for (point t : {p + d, p - d}) {     // t是切点
    line L2(t, t + (T.b - T.a).perp());// L2是直径
    point center = intersection(L, L2);
    double r = len(center, a);
    circle c(center, r);
    //    GD_LAYER() << "Rreal Circle";
    //    GD_CIRCLE(c.center.x, c.center.y, c.r);
    ans.push_back(circle(center, r));
  }
  return ans;
}

// 由圆上一点和两条切线确定圆
std::vector<circle> get_circles_by_ptt(point p, line T1, line T2) {
  if (collinear(T1.a, T1.b, T2.a, T2.b))
    return {};
  vector<line> bs = bisectors(T1, T2);
  std::vector<circle> ans;
  for (line L : bs) {// L是直径所在的直线
    if (L.contains(p)) {
      if (parallel(T1, T2)) {
        double r = dist(p, T1);
        vec d = r * L.unit();
        ans.push_back(circle(p - d, r));
        ans.push_back(circle(p + d, r));
      } else {
        double d = dist(p, T1);
        point a = intersection(T1, T2);// 两切线的交点。
        double l = len(p, a);
        {
          // case#1: d > r
          double r = d * l / (l + d);
          point c = p + r * (a - p).unit();
          ans.push_back(circle(c, r));
        }
        {
          // case#2 d < r
          double r = d * l / (l - d);
          point c = p + r * (p - a).unit();
          ans.push_back(circle(c, r));
        }
      }
    } else {
      point q = mirror(p, L);
      auto tmp = get_circles_by_ppt(p, q, T1);
      ans.insert(ans.end(), tmp.begin(), tmp.end());
    }
  }
  return ans;
}

// 由三条切线确定圆
std::vector<circle> get_circles_by_ttt(line L1, line L2, line L3) {
  if (parallel(L1, L2) && parallel(L1, L3))
    return {};
  std::vector<circle> ans;
  for (line d1: bisectors(L1, L2))
    for (line d2: bisectors(L1, L3))
      if (!parallel(d1, d2)) {
        point center = intersection(d1, d2);
        double r = dist(center, L1);
        ans.push_back({center, r});
      }
  return ans;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GEOMETRY_HPP_
