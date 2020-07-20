//
// Created by zjsdu on 5/17/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
#define JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
#include <cassert>
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
template<typename Int, typename Check>
Int binary_search(Int L, Int R, const Check &check, int type = 0,
                  bool knowingly = false) {
  // check returns: -1, too small; 1, too large; 0, OK
  // type: -1, find first; 0, find any; 1, find last
  assert(L <= R);
  Int not_found = R + 1;
  bool found = false;
  while (L + knowingly <= R) {
    Int mid = L + (R - L) / 2;
    int res = check(mid);
    if (res == -1)
      L = mid + 1;
    else if (res == 1)
      R = mid - 1;
    else {
      if (type == 0)
        return mid;
      found = true;
      type == -1 ? R = mid - 1 : L = mid + 1;
    }
  }
  if (knowingly)
    return L;
  if (!found)
    return not_found;
  return type == -1 ? L : R;
}
#endif// JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
