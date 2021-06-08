//
// Created by zjsdu on 5/2/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
#include <array>
#include <queue>
#include <utility>
#include <type_traits>

template<typename T, typename = void>
struct rank_ : public std::integral_constant<std::size_t, 0> {};

template<typename T>
struct rank_<T, std::void_t<decltype(std::declval<T>()[0])>>
    : public std::integral_constant<
          std::size_t, rank_<decltype(std::declval<T>()[0])>::value + 1> {};

template<typename T, typename = void> struct remove_all_extents_ {
  typedef std::remove_reference_t<T> type;
};

template<typename T>
struct remove_all_extents_<T, std::void_t<decltype(std::declval<T>()[0])>> {
  typedef
      typename remove_all_extents_<decltype(std::declval<T>()[0])>::type type;
};

template<typename DistanceArray> class dijkstra {
  DistanceArray &d;// TODO: Is it possible make d a void*?
  typedef typename remove_all_extents_<DistanceArray>::type Distance;
  static constexpr std::size_t Dimension = rank_<DistanceArray>::value;
  struct S {
    Distance distance;
    std::array<int, Dimension> index;
    bool operator<(const S &other) const { return distance > other.distance; }
    template<typename T> struct tuple_factory_;
    template<std::size_t... Is>
    struct tuple_factory_<std::index_sequence<Is...>> {
      static auto to_tuple(const S &s) {
        return std::tuple(s.distance, s.index[Is]...);
      }
    };

    auto as_tuple() const {
      return tuple_factory_<std::make_index_sequence<Dimension>>::to_tuple(
          *this);
    }
  };
  std::priority_queue<S> q;
  template<typename T> Distance &get_distance_(T &dis, int index) {
    return dis[index];
  }
  template<typename T, typename... Ts>
  Distance &get_distance_(T &dis, int index, Ts... indices) {
    return get_distance_(dis[index], indices...);
  }
  template<typename... Ts> Distance &get_distance(Ts... indices) {
    return get_distance_(d, indices...);
  }

 public:
  dijkstra(DistanceArray &d) : d(d) {}
  template<typename... Ts> void relax(Distance distance, Ts... indices) {
    Distance &dis = get_distance(indices...);
    if (dis > distance) {
      dis = distance;
      q.push({dis, std::array<int, Dimension>{indices...}});
    }
  }
  bool empty() {
    while (not q.empty()
           and q.top().distance
               != std::apply(
                   [this](auto... indices) { return get_distance(indices...); },
                   q.top().index)) {
      q.pop();
    }
    return q.empty();
  }
  auto pop() {
    auto result = q.top();
    q.pop();
    return result.as_tuple();
  }
};
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
