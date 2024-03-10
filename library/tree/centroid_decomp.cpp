//
// Created by zjs on 3/9/24.
//

const int maxn = 2e5 + 5;
vector<int> g[maxn];
bool del[maxn];
int sz[maxn];

int get_centroid(int desired, int node, int parent = -1) {
  for (int i : g[node])
    if (!del[i] && i != parent && sz[i] > desired)
      return get_centroid(desired, i, node);
  return node;
}

int get_sz(int u, int p = 0) {
  sz[u] = 1;
  for (int i : g[u])
    if (!del[i] && i != p)
      sz[u] += get_sz(i, u);
  return sz[u];
}
// 以上代码不要改动

void solve(int u = 1) {
  int c = get_centroid(get_sz(u) >> 1, u);
  del[c] = true;

  // do somthing to the tree rooted at c

  for (int v : g[c]) {
    if (!del[v]) {
      solve(v);
    }
  }
}
