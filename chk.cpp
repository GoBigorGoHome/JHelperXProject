//
// Created by zjs on 10/12/23.
//

#include "testlib.h"
#include <algorithm>
using namespace std;

int main(int argc, char *argv[]) {
  setName("Check permutation");
  registerTestlibCmd(argc, argv);
  int s = inf.readInt();
  int t = inf.readInt();
  int m = inf.readInt();
  set<pair<int,int>> edges;
  while (m--) {
    int u = inf.readInt();
    int v = inf.readInt();
    edges.insert({u, v});
  }
  int first = ans.readInt();
  if (first == -1) {
    int k = ouf.readInt();
    if (k != -1)
      quitf(_wa, "There is no 4-cycle.");
    quitf(_ok, "There is no 4-cycle.");
  }

  vector<int> p = ouf.readInts(4, 1, s + t);
  sort(p.begin(), p.end());
  for (int i = 0; i < 3; i++)
    if (p[i] == p[i + 1]) {
      quitf(_wa, "There are duplicated vertices.");
    }

  for (int i = 0; i < 2; i++)
    for (int j = 2; j < 4; j++)
      if (edges.count({p[i], p[j]}) == 0)
        quitf(_wa, "Not a cycle.");

  quitf(_ok, "A 4-cycle is found.");
}