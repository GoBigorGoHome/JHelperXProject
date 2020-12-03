//
// Created by zjsdu on 5/17/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
#define JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
#include <cassert>
#include <optional>
#include <midpoint.hpp>
template<typename Compare>
int ternary_search(int L, int R, const Compare &compare) {
  assert(L <= R);
  // mid 是 [L,R] 的中点，mm 是 [mid+1,R] 的中点，
  // 又 L < R，这就保证了 L <= mid < mm <= R。
  while (L < R) {
    int mid = L + (R - L) / 2, mm = mid + 1 + (R - mid - 1) / 2;
    if (compare(mid, mm)) {
      // reduce search range by one fourth
      R = mm - 1;
    } else {
      // reduce search range by half
      L = mid + 1;
    }
  }
  assert(L == R);
  return L;
}
//! @param check returns: -1, too small; 1, too large; 0, OK
//! @param type -1, find first; 0, find any; 1, find last
//! @param knowingly true if we know that at least one integer in the range [L,
//! R] is OK
template<typename Int, typename Check>
std::optional<Int> bin_search(Int L, Int R, const Check &check, int type = 0,
                              bool knowingly = false) {
  Int l = L, r = R;
  std::optional<Int> ans;
  while (l <= r) {
    if (l == r and knowingly) {
      ans = l;
      break;
    }
    Int m = midpoint(l, r);
    int res = check(m);
    if (res == -1) {
      if (m == R)
        break;
      l = m + 1;
    } else if (res == 1) {
      if (m == L)
        break;
      r = m - 1;
    } else {
      ans = m;
      if (type == 0)
        break;
      knowingly = false;
      if (type == -1) {
        if (m == L)
          break;
        else
          r = m - 1;
      } else {
        if (m == R)
          break;
        else
          l = m + 1;
      }
    }
  }
  return ans;
}
template<typename Int, typename Check>
std::optional<Int> find_first(Int L, Int R, const Check &check) {
  std::optional<Int> ans;
  Int l = L, r = R;
  while (l <= r) {
    Int m = midpoint(l, r);
    if (check(m)) {
      ans = m;
      if (m == L)
        break;
      r = m - 1;
    } else {
      if (m == R)
        break;
      l = m + 1;
    }
  }
  return ans;
}
template<typename Int, typename Check>
std::optional<Int> find_last(Int L, Int R, const Check &check) {
  std::optional<Int> ans;
  Int l = L, r = R;
  while (l <= r) {
    Int m = midpoint(l, r);
    if (check(m)) {
      ans = m;
      if (m == R)
        break;
      l = m + 1;
    } else {
      if (m == L)
        break;
      r = m - 1;
    }
  }
  return ans;
}
#endif// JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
