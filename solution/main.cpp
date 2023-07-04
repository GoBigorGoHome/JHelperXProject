//
// Created by zjs on 7/4/23.
//
void solve();

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>

void write_tellg() {
  std::ofstream f("solution/tellg.txt");
  f << std::cin.tellg();
}

void write_tellg_abort() {
  write_tellg();
  std::abort();
}

int main(int argc, char **argv) {
  std::set_terminate(write_tellg_abort);
  std::atexit(write_tellg);
  std::cin.seekg(std::stoi(argv[1]));
  solve();
  return 0;
}