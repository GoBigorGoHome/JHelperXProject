//
// Created by zjsdu on 5/2/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
#include <array>
#include <queue>
#include <ndarray.hpp>
template<typename Distance, unsigned Dimension, typename DistanceArray>
class dijkstra {
  DistanceArray &d;// TODO: Is it possible make d a void*?
  struct S {
    Distance distance;
    std::array<int, Dimension> index;
    bool operator<(const S &other) const { return distance > other.distance; }
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
  bool empty() const { return q.empty(); }
  S pop() {
    auto result = q.top();
    q.pop();
    while (not q.empty()
           and q.top().distance
               != std::apply(
                   [this](auto... indices) { return get_distance(indices...); },
                   q.top().index)) {
      q.pop();
    }
    return result;
  }
};

template<typename Distance>
using dij = dijkstra<Distance, 1, std::vector<Distance>>;
template<typename Distance>
using dij2 = dijkstra<Distance, 2, ndarray<2, Distance>>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_DIJKSTRA_HPP_
