//
// Created by zjsdu on 8/4/2020.
//
#include "compare_floats.h"
#include <cmath>
#include <sstream>

const long double EPS = 1E-6;

bool compare_floats(const std::string &output, const std::string &answer) {
  std::stringstream expect(answer), out(output);
  long double x = 0, y = 0;
  while (expect >> x) {
    if (not(out >> y))
      return false;
    if (std::isinf(y) or std::isnan(y))
      return false;
    if (std::fabs(x - y) < EPS)
      continue;
    if (std::fabs(x - y) < std::fabs(x) * EPS)
      continue;
    return false;
  }
  return true;
}