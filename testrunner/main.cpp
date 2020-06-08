#include "tester.h"
#include "debug.hpp"
#include "../tasks/DSolveTheMaze.cpp"
#include <chrono>

std::vector<jhelper::Test> tests{{"6\n1 1\n.\n1 2\nG.\n2 2\n#B\nG.\n2 3\nG.#\nB#.\n3 3\n#B.\n#..\nGG.\n2 2\n#B\nB.\n", "Yes\nYes\nNo\nNo\nYes\nYes\n", true, true},};
#define in std::cin
#define out std::cout
double solver_call() {
  DSolveTheMaze solver;
  auto start = std::chrono::steady_clock::now();
  int n;
in >> n;
for(int i = 0; i < n; ++i) {
	solver.solve(in, out);
}

  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  return elapsed_seconds.count();
}

int main() {
  jhelper::test_runner();
  return 0;
}
