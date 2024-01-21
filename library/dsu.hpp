#include <vector>
#include <algorithm>

class dsu {
  std::vector<int> parent_or_size;
  int nTree;
  std::vector<bool> has_cycle_;

 public:
  explicit dsu(int n)
      : parent_or_size(n, -1), nTree(n), has_cycle_(n) {// 0-indexed
  }

  void reset() {
    std::fill(parent_or_size.begin(), parent_or_size.end(), -1);
    std::fill(has_cycle_.begin(), has_cycle_.end(), false);
    nTree = (int) parent_or_size.size();
  }

  int n_tree() const { return nTree; }

  int size(int x) { return -parent_or_size[leader(x)]; }

  /// @brief Reset the size of the tree where x belongs to 1. This method is not
  /// for general use.
  void reset_size(int x) { parent_or_size[leader(x)] = -1; }

  bool has_cycle(int x) { return has_cycle_[leader(x)]; }

  int leader(int x) {
    return parent_or_size[x] < 0
        ? x
        : parent_or_size[x] = leader(parent_or_size[x]);
  }
  //! @brief Merge tree y into tree x.
  //! @param f Function for extra processing, called as f(x, y).
  template<typename F = void (*)(int, int)>
  bool merge(
      int x, int y, const F &f = [](int, int) {}) {
    int rx = leader(x), ry = leader(y);
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

  bool is_leader(int x) const { return parent_or_size[x] < 0; }

  bool same(int x, int y) { return leader(x) == leader(y); }
};
