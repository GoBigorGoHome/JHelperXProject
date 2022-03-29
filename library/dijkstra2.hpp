//
// Created by zjsdu on 2021/12/22.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA2_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA2_HPP_

#include <array>
#include <queue>
#include <utility>
#include <tuple>
#include <cassert>
#include <limits>
#include <ndarray.hpp>

/// @pre DistanceType has operator<
template<typename DistanceType, unsigned Dimension> class dijkstra {

  ndarray<Dimension, DistanceType> dis_array;

  struct S {
    DistanceType distance;
    std::array<int, Dimension> index;
    bool operator<(const S& other) const { return  other.distance < distance; }

    template<typename T> struct tuple_factory_;

    template<std::size_t... Is>
    struct tuple_factory_<std::index_sequence<Is...>> {
      static auto to_tuple(const S& s) {
        return std::tuple(s.distance, s.index[Is]...);
      }
    };

    auto as_tuple() const {
      return tuple_factory_<std::make_index_sequence<Dimension>>::to_tuple(
          *this);
    }
  };

  std::priority_queue<S> q;

  template<typename T> DistanceType& get_distance_(T& dis, int index) {
    return dis[index];
  }

  template<typename T, typename... Ts>
  DistanceType& get_distance_(T& dis, int index, Ts... indices) {
    return get_distance_(dis[index], indices...);
  }

  template<typename... Ts> DistanceType& get_distance(Ts... indices) {
    return get_distance_(dis_array, indices...);
  }

 public:
  template<typename... Ints>
  explicit dijkstra(Ints... lens)
      : dis_array(lens..., std::numeric_limits<DistanceType>::max()) {
    static_assert(sizeof...(Ints) == Dimension);
  }

  template<typename... Ints> bool relax(DistanceType d, Ints... indices) {
    static_assert(sizeof...(Ints) == Dimension);
    DistanceType& dis = get_distance(indices...);
    if (d < dis) {
      dis = d;
      q.push({dis, std::array<int, Dimension>{indices...}});
      return true;
    }
    return false;
  }

  bool empty() const { return q.empty(); }

  /// @returns a tuple (distance, indices...)
  auto pop() {
    assert(not q.empty());
    auto result = q.top();
    q.pop();
    while (not q.empty()
           and q.top().distance
               != std::apply(
                   [this](auto... indices) {
                     return this->get_distance(indices...);
                   },
                   q.top().index)) {
      q.pop();
    }
    return result.as_tuple();
  }

  // TODO: Make this member function const. That seems a lot of work.
  template<typename... Ints> DistanceType distance(Ints... indices) {
    static_assert(sizeof...(Ints) == Dimension);
    return get_distance(indices...);
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA2_HPP_
