#include <cassert>
#include <cstring>
#include <queue>
#include <vector>
// ftiasch's AC automaton template
template<int C, char alpha, int N>
struct AhoCorasick {
  AhoCorasick() {
    weight.reserve(N);
    go.reserve(N);
    go.emplace_back(C, -1);
    weight.emplace_back(0);
  }

  void insert(const char *s, int w = 1) {
    int p = 0;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - alpha;
      if (go[p][c] == -1) {
        go[p][c] = go.size();
        go.emplace_back(C, -1);
        weight.emplace_back(0);
      }
      p = go[p][c];
      weight[p] += w;
    }
  }

  int get(const char *s) const {
    int p = 0;
    for (int i = 0; s[i]; ++i) {
      int c = s[i] - alpha;
      if (go[p][c] == -1) { return -1; }
      p = go[p][c];
    }
    return p;
  }

  void initialize() {
    fail.resize(go.size());
    std::queue<int> queue;
    for (int c = 0; c < C; ++c) {
      int &v = go[0][c];
      if (~v) { queue.push(v); }
      (~v ? fail[v] : v) = 0;
    }
    while (!queue.empty()) {
      int u = queue.front();
      queue.pop();
      weight[u] += weight[fail[u]];
      for (int c = 0; c < C; ++c) {
        int &v = go[u][c];
        if (~v) { queue.push(v); }
        (~v ? fail[v] : v) = go[fail[u]][c];
      }
    }
  }

  std::vector<int> weight;
  std::vector<std::vector<int>> go;
  std::vector<int> fail;
};

template<typename RAIter>
class kmp_searcher {
  RAIter pattern_, pattern_end_;
  int pattern_len_;
  std::vector<int> fail_;
  template<typename T>
  int get_next(T character, int i) const {
    while (i != -1 && pattern_[i] != character) i = fail_[i];
    return i + 1;
  }
  void calc_fail() {
    auto f1 = fail_.begin();
    auto f2 = f1 + 1;
    *f1 = -1;
    for (RAIter it = pattern_; f2 != fail_.end(); ++f2)
      *f2 = get_next(*it++, *f1++);
  }

 public:
  kmp_searcher(RAIter pattern, RAIter pattern_end)
      : pattern_(pattern), pattern_end_(pattern_end) {
    pattern_len_ = pattern_end_ - pattern_;
    assert(pattern_len_ > 0);
    fail_.assign(pattern_len_ + 1, 0);
    calc_fail();
  }
  template<typename RAIter2>
  std::pair<RAIter2, RAIter2> operator()(RAIter2 first, RAIter2 last) const {
    int j = 0;
    for (RAIter2 it = first; it != last; ++it) {
      j = get_next(*it, j);
      if (j == pattern_len_) { return {it + 1 - pattern_len_, it + 1}; }
    }
    return {last, last};
  }
};
