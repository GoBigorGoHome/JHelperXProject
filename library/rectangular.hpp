//
// Created by zjs on 2021/11/25.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_RECTANGULAR_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_RECTANGULAR_HPP_
#include <vector>
#include <cassert>
#include <monotonic_queue.hpp>

template<typename Value, typename Sum = long long>
std::vector<std::vector<Sum>>
rect_sum(const std::vector<std::vector<Value>>& a) {
  assert(a.size() > 0);
  assert(a[0].size() > 0);
  int H = (int) a.size();
  int W = (int) a[0].size();
  std::vector<std::vector<Sum>> sum(H, std::vector<Sum>(W));
  sum[0][0] = a[0][0];
  for (int r = 1; r < H; r++) {
    sum[r][0] = a[r][0] + sum[r - 1][0];
  }
  for (int c = 1; c < W; c++) {
    sum[0][c] = a[0][c] + sum[0][c - 1];
  }
  for (int r = 1; r < H; r++) {
    for (int c = 1; c < W; c++) {
      sum[r][c] = a[r][c] + sum[r - 1][c] + sum[r][c - 1] - sum[r - 1][c - 1];
    }
  }
  return sum;
}

template<typename Value, typename Sum = long long>
std::vector<std::vector<Sum>> rect_sum(const std::vector<std::vector<Value>>& a,
                                       int h, int w) {
  assert(a.size() > 0);
  assert(a[0].size() > 0);
  int H = (int) a.size();
  int W = (int) a[0].size();
  assert(0 < h and h <= H);
  assert(0 < w and w <= W);
  std::vector<std::vector<Sum>> sum = rect_sum(a);
  int H2 = H - h + 1;
  int W2 = W - w + 1;
  std::vector<std::vector<Sum>> sum2(H2, std::vector<Sum>(W2));
  sum2[0][0] = sum[h - 1][w - 1];
  for (int r = 1; r < H2; r++) {
    sum2[r][0] = sum[r + h - 1][w - 1] - sum[r - 1][w - 1];
  }
  for (int c = 1; c < W2; c++) {
    sum2[0][c] = sum[h - 1][c + w - 1] - sum[h - 1][c - 1];
  }
  for (int r = 1; r < H2; r++) {
    for (int c = 1; c < W2; c++) {
      int r2 = r + h - 1;
      int c2 = c + w - 1;
      sum2[r][c] =
          sum[r2][c2] - sum[r2][c - 1] - sum[r - 1][c2] + sum[r - 1][c - 1];
    }
  }
  return sum2;
}

template<typename Value>
std::vector<std::vector<Value>>
rect_max(const std::vector<std::vector<Value>>& a, int h, int w) {
  assert(a.size() > 0);
  assert(a[0].size() > 0);
  int H = (int) a.size();
  int W = (int) a[0].size();
  assert(0 < h and h <= H);
  assert(0 < w and w <= W);
  int H2 = H - h + 1;
  int W2 = W - w + 1;
  std::vector<std::vector<Value>> row_max(H, std::vector<Value>(W2));
  monotonic_queue<Value> q;
  for (int r = 0; r < H; r++) {
    q.clear();
    for (int c = 0; c < w - 1; c++) {
      q.push(a[r][c]);
    }
    for (int c = w - 1; c < W; c++) {
      q.push(a[r][c]);
      row_max[r][c - w + 1] = q.top();
      q.pop(a[r][c - w + 1]);
    }
  }
  std::vector<std::vector<Value>> ans(H2, std::vector<Value>(W2));
  for (int c = 0; c < W2; c++) {
    q.clear();
    for (int r = 0; r < h - 1; r++) {
      q.push(row_max[r][c]);
    }
    for (int r = h - 1; r < H; r++) {
      q.push(row_max[r][c]);
      ans[r - h + 1][c] = q.top();
      q.pop(row_max[r - h + 1][c]);
    }
  }
  return ans;
}

template<typename Value>
std::vector<std::vector<Value>>
rect_min(const std::vector<std::vector<Value>>& a, int h, int w) {
  assert(a.size() > 0);
  assert(a[0].size() > 0);
  int H = (int) a.size();
  int W = (int) a[0].size();
  assert(0 < h and h <= H);
  assert(0 < w and w <= W);
  int H2 = H - h + 1;
  int W2 = W - w + 1;
  std::vector<std::vector<Value>> row_min(H, std::vector<Value>(W2));
  monotonic_queue<Value, std::greater<Value>> q;
  for (int r = 0; r < H; r++) {
    q.clear();
    for (int c = 0; c < w - 1; c++) {
      q.push(a[r][c]);
    }
    for (int c = w - 1; c < W; c++) {
      q.push(a[r][c]);
      row_min[r][c - w + 1] = q.top();
      q.pop(a[r][c - w + 1]);
    }
  }
  std::vector<std::vector<Value>> ans(H2, std::vector<Value>(W2));
  for (int c = 0; c < W2; c++) {
    q.clear();
    for (int r = 0; r < h - 1; r++) {
      q.push(row_min[r][c]);
    }
    for (int r = h - 1; r < H; r++) {
      q.push(row_min[r][c]);
      ans[r - h + 1][c] = q.top();
      q.pop(row_min[r - h + 1][c]);
    }
  }
  return ans;
}

#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_RECTANGULAR_HPP_
