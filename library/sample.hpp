//
// Created by zjsdu on 7/3/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_SAMPLE_HPP_
#include <unordered_set>
#include <random>
#include <algorithm>
std::unordered_set<int> pickSet(int N, int k, std::mt19937 &gen) {
  std::unordered_set<int> elems;
  for (int r = N - k; r < N; ++r) {
    int v = std::uniform_int_distribution<>(0, r)(gen);

    // there are two cases.
    // v is not in candidates ==> add it
    // v is in candidates ==> well, r is definitely not, because
    // this is the first iteration in the loop that we could've
    // picked something that big.
    // Proof that each number of 0..n-1 is has the same chance k/n to be picked.

    if (!elems.insert(v).second) {
      elems.insert(r);
    }
  }
  return elems;
}

std::vector<int> pick(int N, int k) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::unordered_set<int> elems = pickSet(N, k, gen);

  // ok, now we have a set of k elements. but now
  // it's in a [unknown] deterministic order.
  // so we have to shuffle it:

  std::vector<int> result(elems.begin(), elems.end());
  std::shuffle(result.begin(), result.end(), gen);
  return result;
}
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_SAMPLE_HPP_

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_SAMPLE_HPP_
