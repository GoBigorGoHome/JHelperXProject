#include <vector>
#include <algorithm>

class UnionFind {
  std::vector<int> parent_or_size;
  int nTree;
  std::vector<bool> has_cycle_;

 public:
  explicit UnionFind(int n)
      : parent_or_size(n, -1), nTree(n), has_cycle_(n) {// 0-indexed
  }
  void init() {
    std::fill(parent_or_size.begin(), parent_or_size.end(), -1);
    nTree = (int) parent_or_size.size();
  }
  int n_tree() const { return nTree; }

  int size(int x) { return -parent_or_size[root(x)]; }

  bool has_cycle(int x) { return has_cycle_[root(x)]; }

  int root(int x) {
    return parent_or_size[x] < 0 ? x
                                 : parent_or_size[x] = root(parent_or_size[x]);
  }
  //! @brief Merge tree y into tree x.
  //! @param f Function for extra processing, called as f(x, y).
  template<typename F = void (*)(int, int)>
  bool unite(
      int x, int y, const F &f = [](int, int) {}) {
    int rx = root(x), ry = root(y);
    if (rx != ry) {
      parent_or_size[rx] += parent_or_size[ry];
      parent_or_size[ry] = rx;
      --nTree;
      has_cycle_[rx] = has_cycle_[rx] or has_cycle_[ry];
      f(rx, ry);
      return true;
    }
    has_cycle_[rx] = true;
    return false;
  }

  bool is_root(int x) const { return parent_or_size[x] < 0; }
  bool same(int x, int y) { return root(x) == root(y); }
};
