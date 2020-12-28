//
// Created by zjsdu on 12/22/2020.
//
#include "print_test.h"
#include "color.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "test.h"
#include "string_utils.h"

extern std::ostringstream debug_stream;
extern std::ostringstream diagnostic_stream;
extern std::vector<jhelper::Test> tests;

namespace jhelper {
void print_test(std::ostream &os, int test_id, const std::string &task_output) {
  std::string debug_info = debug_stream.str();
  int n_line = 0;
  auto end =
      std::find_if(debug_info.begin(), debug_info.end(),
                   [&n_line](char x) { return x == '\n' && ++n_line == 80; });

  os << BRIGHT_BLACK << debug_info.substr(0, end - debug_info.begin()) << RESET
     << '\n';
  if (end != debug_info.end())
    os << YELLOW "TRUNCATED DEBUG INFO\n" RESET;

  auto &test = tests[test_id];
  os << "Test #" << test_id << '\n';
  os << "Input: \n"
     << (line_cnt(test.input) > 100 ? BRIGHT_BLACK "TOO LONG, SKIPPED" RESET
                                    : test.input)
     << '\n';
  if (test.has_output)
    os << "Expected output: \n" << test.output << '\n';
  else
    os << "Expected output: \nN/A\n";

  os << "Actual output: \n"
     << BRIGHT_WHITE
     << (line_cnt(task_output) > 2000 ? "TOO LONG, SKIPPED" : task_output)
     << RESET << '\n';
  os << YELLOW << diagnostic_stream.str() << RESET;
  os << BLUE "====================================\n" RESET;
  os.flush();
}

void print_subtest(std::ostream &os, int test_id, int subtest_id,
                   long long input_pos,
                   std::vector<std::string>::const_iterator b,
                   std::vector<std::string>::const_iterator e,
                   const std::string &task_output) {
  std::string debug_info = debug_stream.str();
  int n_line = 0;
  auto end =
      std::find_if(debug_info.begin(), debug_info.end(),
                   [&n_line](char x) { return x == '\n' && ++n_line == 80; });

  os << BRIGHT_BLACK << debug_info.substr(0, end - debug_info.begin()) << RESET
     << '\n';
  if (end != debug_info.end())
    os << YELLOW "TRUNCATED DEBUG INFO\n" RESET;

  auto &test = tests[test_id];
  os << "Subtest #" << test_id << "." << subtest_id << '\n';
  os << "Input: \n";
  auto cur_input_pos = std::cin.tellg();
  if (cur_input_pos == -1) {
    os << test.input + input_pos << '\n';
  } else {
    while (input_pos < cur_input_pos) {
      os << test.input[input_pos];
      ++input_pos;
    }
    os << '\n';
  }
  os << '\n';
  os << "Expected output: \n";
  while (b != e) {
    os << *b << '\n';
    ++b;
  }
  os << '\n';
  os << "Actual output: \n"
     << BRIGHT_WHITE
     << (line_cnt(task_output) > 2000 ? "TOO LONG, SKIPPED" : task_output)
     << RESET << '\n';
  os << YELLOW << diagnostic_stream.str() << RESET;
  os << BLUE "====================================\n" RESET;
  os.flush();
}
}// namespace jhelper
