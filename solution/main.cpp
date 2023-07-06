//
// Created by zjs on 7/4/23.
//
void solve();

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <csignal>

void write_tellg() {
  std::ofstream f("solution/tellg.txt");
  f << std::cin.tellg();
}

void write_tellg_sigterm(int sig) {
  write_tellg();
}

int main(int argc, char **argv) {
  std::signal(SIGTERM, write_tellg_sigterm);
  std::atexit(write_tellg);
  if (argc > 1)
    std::cin.seekg(std::stoi(argv[1]));
  solve();
  return 0;
}