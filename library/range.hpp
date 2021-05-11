//
// Created by zjsdu on 5/11/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_RANGE_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_RANGE_HPP_
#include <cassert>
#include <utility>
#include <algorithm>
template<typename iterator> struct range {
  iterator beg_, end_;
  iterator begin() const { return beg_; }
  iterator end() const { return end_; }
  //! @brief Make a range from a pair of iterators.
  range(iterator beg, iterator end) : beg_(beg), end_(end) {}
};

template<typename Sequence>
struct slice : public range<decltype(std::begin(std::declval<Sequence &>()))> {
  slice(Sequence &s, std::size_t l, std::size_t r)
      : range<decltype(std::begin(std::declval<Sequence &>()))>(
          std::begin(s) + std::min(l, std::size(s)),
          std::begin(s) + std::min(r, std::size(s))) {
    assert(l <= r);
  }
};

template<typename Sequence> struct first : public slice<Sequence> {
  first(Sequence &s, std::size_t len)
      : slice<Sequence>(s, 0, std::min(std::size(s), len)) {}
};

template<typename Sequence> struct last : public slice<Sequence> {
  last(Sequence &s, std::size_t len)
      : slice<Sequence>(s, std::end(s) - std::min(len, std::size(s)),
                        std::end(s)){};
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_RANGE_HPP_
