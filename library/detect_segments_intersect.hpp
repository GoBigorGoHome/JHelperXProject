//
// Created by zjsdu on 5/30/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DETECT_SEGMENTS_INTERSECT_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DETECT_SEGMENTS_INTERSECT_HPP_
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
const double EPS = 1E-9;

struct pnt {
  double x, y;
  double len() const { return std::sqrt(x * x + y * y); }
};
struct seg {
  pnt p, q;
  int id;

  double get_y(double x) const {
    if (std::abs(p.x - q.x) < EPS)
      return p.y;
    return p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
  }
};

bool intersect1d(double l1, double r1, double l2, double r2) {
  if (l1 > r1)
    std::swap(l1, r1);
  if (l2 > r2)
    std::swap(l2, r2);
  return std::max(l1, l2) <= std::min(r1, r2) + EPS;
}

int vec(const pnt &a, const pnt &b, const pnt &c) {
  double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
  return std::abs(s) < EPS ? 0 : s > 0 ? +1 : -1;
}

bool intersect(const seg &a, const seg &b) {
  return intersect1d(a.p.x, a.q.x, b.p.x, b.q.x)
      && intersect1d(a.p.y, a.q.y, b.p.y, b.q.y)
      && vec(a.p, a.q, b.p) * vec(a.p, a.q, b.q) <= 0
      && vec(b.p, b.q, a.p) * vec(b.p, b.q, a.q) <= 0;
}

bool operator<(const seg &a, const seg &b) {
  double x = std::max(std::min(a.p.x, a.q.x), std::min(b.p.x, b.q.x));
  return a.get_y(x) < b.get_y(x) - EPS;
}

struct event {
  double x;
  int tp, id;

  event() {}
  event(double x, int tp, int id) : x(x), tp(tp), id(id) {}

  bool operator<(const event &e) const {
    if (std::abs(x - e.x) > EPS)
      return x < e.x;
    return tp > e.tp;
  }
};

std::pair<int, int> detect_intersect(const std::vector<seg> &a) {
  int n = (int) a.size();
  std::vector<event> e;
  for (int i = 0; i < n; ++i) {
    e.push_back(event(std::min(a[i].p.x, a[i].q.x), +1, i));
    e.push_back(event(std::max(a[i].p.x, a[i].q.x), -1, i));
  }
  std::sort(e.begin(), e.end());
  std::set<seg> s;
  std::vector<std::set<seg>::iterator> where(n);
  auto prev = [&s](std::set<seg>::iterator it) {
    return it == s.begin() ? s.end() : --it;
  };

  for (const auto &ev : e) {
    int id = ev.id;
    if (ev.tp == +1) {
      std::set<seg>::iterator nxt = s.lower_bound(a[id]), prv = prev(nxt);
      if (nxt != s.end() && intersect(*nxt, a[id]))
        return std::make_pair(nxt->id, id);
      if (prv != s.end() && intersect(*prv, a[id]))
        return std::make_pair(prv->id, id);
      where[id] = s.insert(nxt, a[id]);
      if (where[id]->id != id) {
        return {where[id]->id, id};
      }
    } else {
      std::set<seg>::iterator nxt = std::next(where[id]), prv = prev(where[id]);
      if (nxt != s.end() && prv != s.end() && intersect(*nxt, *prv))
        return std::make_pair(prv->id, nxt->id);
      s.erase(where[id]);
    }
  }

  return std::make_pair(-1, -1);
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DETECT_SEGMENTS_INTERSECT_HPP_
