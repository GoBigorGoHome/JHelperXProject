#include <vector>
#include <numeric>

class UnionFind {
  std::vector<int> par, sz;
  int nTree;

 public:
  explicit UnionFind(int n) : par(n), sz(n, 1), nTree(n) {// 0-indexed
    std::iota(par.begin(), par.end(), 0);
  }
  void init() {
    std::fill(par.begin(), par.end(), 1);
    nTree = (int) par.size();
    std::iota(par.begin(), par.end(), 0);
  }
  int n_tree() const { return nTree; }

  int size(int x) { return sz[root(x)]; }

  int root(int x) { return x == par[x] ? x : par[x] = root(par[x]); }
  //! @brief Merge tree y into tree x.
  //! @param f Function for extra processing, called as f(x, y).
  template<typename F = void (*)(int, int)>
  bool unite(
      int x, int y, const F &f = [](int, int) {}) {
    int rx = root(x), ry = root(y);
    if (rx != ry) {
      par[ry] = rx;
      --nTree;
      sz[rx] += sz[ry];
      f(rx, ry);
      return true;
    }
    return false;
  }

  bool is_root(int x) const { return par[x] == x; }
  bool same(int x, int y) { return root(x) == root(y); }
};