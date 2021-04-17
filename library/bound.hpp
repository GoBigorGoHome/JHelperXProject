//
// Created by zjsdu on 7/21/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_BOUND_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_BOUND_HPP_
#include <algorithm>
// upper_bound: *ptr <= val | val < *ptr
// lower_bound: *ptr < val | val <= *ptr
// [ lower_bound(beg, end, val), upper_bound(beg, end, val) ) is
// the sub-range of elements that "equal" to val.
template<typename Sequence, typename Value>
int lb(const Sequence &seq, const Value &val) {
  return static_cast<int>(std::lower_bound(std::begin(seq), std::end(seq), val)
                          - std::begin(seq));
}
template<typename Sequence, typename Value>
int ub(const Sequence &seq, const Value &val) {
  return static_cast<int>(std::upper_bound(std::begin(seq), std::end(seq), val)
                          - std::begin(seq));
}
template<typename Sequence, typename Value, typename Compare>
int lb(const Sequence &seq, const Value &val, Compare comp) {
  return static_cast<int>(
      std::lower_bound(std::begin(seq), std::end(seq), val, comp)
      - std::begin(seq));
}
template<typename Sequence, typename Value, typename Compare>
int ub(const Sequence &seq, const Value &val, Compare comp) {
  return static_cast<int>(
      std::upper_bound(std::begin(seq), std::end(seq), val, comp)
      - std::begin(seq));
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_BOUND_HPP_
