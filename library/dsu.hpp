#include <vector>
#include <numeric>

class UnionFind {
  std::vector<int> par, sz;
  int nTree;

public:
  explicit UnionFind(int n) : par(n), sz(n, 1), nTree(n) { // 0-indexed
    std::iota(par.begin(), par.end(), 0);
  }
  void init() {
    std::fill(par.begin(), par.end(), 1);
    nTree = par.size();
    std::iota(par.begin(), par.end(), 0);
  }
  int n_tree() const { return nTree; }

  int size(int x) { return sz[root(x)]; }

  int root(int x) { return x == par[x] ? x : par[x] = root(par[x]); }

  bool unite(int x, int y) {
    int rx = root(x), ry = root(y);
    if (rx != ry) {
      par[rx] = ry;
      --nTree;
      sz[ry] += sz[rx];
      return true;
    }
    return false;
  }

  bool is_root(int x) const { return par[x] == x; }
  bool same(int x, int y) { return root(x) == root(y); }
};