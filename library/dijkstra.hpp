//
// Created by zjsdu on 5/2/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
#include <array>
#include <queue>
#include <utility>
#include <tuple>
#include <cassert>
#include <type_traits.hpp>

template<typename DistanceArray> class dijkstra {
  DistanceArray& d;// TODO: Is it possible make d a void*?
  static constexpr std::size_t Dimension = rank_<DistanceArray>::value;

  using Distance = typename remove_all_extents_<DistanceArray>::type;

  struct S {
    Distance distance;
    std::array<int, Dimension> index;

    bool operator<(const S& other) const { return distance > other.distance; }

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

  template<typename T> Distance& get_distance_(T& dis, int index) {
    return dis[index];
  }

  template<typename T, typename... Ts>
  Distance& get_distance_(T& dis, int index, Ts... indices) {
    return get_distance_(dis[index], indices...);
  }

  template<typename... Ts> Distance& get_distance(Ts... indices) {
    return get_distance_(d, indices...);
  }

 public:
  explicit dijkstra(DistanceArray& d) : d(d) {}

  template<typename... Ts> void relax(Distance distance, Ts... indices) {
    Distance& dis = get_distance(indices...);
    if (dis > distance) {
      dis = distance;
      q.push({dis, std::array<int, Dimension>{indices...}});
    }
  }

  bool empty() const { return q.empty(); }

  auto pop() {
    assert(not q.empty());
    auto result = q.top();
    q.pop();
    while (not q.empty()
           and q.top().distance
               != std::apply(
                   [this](auto... indices) { return get_distance(indices...); },
                   q.top().index)) {
      q.pop();
    }
    return result.as_tuple();
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
