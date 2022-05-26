//
// Created by zjsdu on 2022/5/26.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_ITERATOR_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_ITERATOR_HPP_
template<typename Container> class Iterator : public Container::iterator {
  typename Container::iterator end;

 public:
  Iterator(Container& container)
      : Container::iterator(container.begin()), end(container.end()) {}
  operator bool() const { return *this != end; }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_ITERATOR_HPP_
