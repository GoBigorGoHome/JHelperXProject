//
// Created by zjsdu on 12/22/2020.
//
#include "print_test.h"
#include "color.h"
#include <iostream>
#include "test.h"
#include "string_utils.h"
#include <cstring>

extern std::vector<jhelper::Test> tests;

namespace jhelper {
void print_test(std::ostream &os, int test_id, const std::string &task_output) {
  auto &test = tests[test_id - 1];
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
  //    os << YELLOW << diagnostic_stream.str() << RESET;
  os << BLUE "====================================\n" RESET;
  os.flush();
}

void print_subtest(std::ostream &os, int test_id, int subtest_id, int old_tellg,
                   int new_tellg, std::vector<std::string>::const_iterator b,
                   std::vector<std::string>::const_iterator e,
                   const std::vector<std::string> &output_lines) {

  os << "Subtest #" << test_id << "." << subtest_id << '\n';
  os << "Input: \n";
  // trim input
  const char *s = tests[test_id - 1].input;
  int l = old_tellg;
  int r = new_tellg == -1 ? (int) strlen(s) : new_tellg;
  while (l < r && std::isspace(s[l]))
    l++;
  while (l < r && std::isspace(s[r - 1]))
    r--;
  for (int i = l; i < r; i++)
    os << s[i];
  os << '\n';

  os << '\n';
  os << "Expected output: \n";
  while (b != e) {
    os << *b << '\n';
    ++b;
  }
  os << '\n';
  os << "Actual output: \n" << BRIGHT_WHITE;
  if (output_lines.size() > 2000)
    os << "TOO LONG, SKIPPED.";
  else
    for (auto &line : output_lines)
      os << line << '\n';
  os << RESET << '\n';
  os << BLUE "====================================\n" RESET;
  os.flush();
}
}// namespace jhelper
