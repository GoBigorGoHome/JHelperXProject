//
// Created by zjsdu on 5/17/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
#define JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
#include <cassert>
template<typename Compare>
int ternary_search(int L, int R, const Compare &compare) {
  assert(L <= R);
  // mid 是 [L,R] 的中点，mm 是 [mid+1,R] 的中点，又 L < R，这就保证了 L <= mid
  // < mm <= R。
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
template<typename Predicate>
int bs_find_first(int L, int R, const Predicate &p, bool knowingly = false) {
  assert(L <= R);
  if (knowingly)
    --R;
  while (L <= R) {
    int mid = L + (R - L) / 2;
    p(mid) ? R = mid - 1 : L = mid + 1;
  }
  return L;
}

template<typename Predicate>
int bs_find_last(int L, int R, const Predicate &p, bool knowingly = false) {
  assert(L <= R);
  if (knowingly)
    ++L;
  while (L <= R) {
    int mid = L + (R - L) / 2;
    p(mid) ? L = mid + 1 : R = mid - 1;
  }
  return R;
}

#endif// JHELPER_EXAMPLE_PROJECT_TASKS_SEARCH_HPP_
