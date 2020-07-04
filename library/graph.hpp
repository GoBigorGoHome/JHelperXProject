//
// Created by zjsdu on 7/5/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
#include <iostream>
template<typename G> void rg(int m, G &g) {
  for (int i = 0, a, b; i < m; ++i) {
    std::cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
}
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_GRAPH_HPP_
