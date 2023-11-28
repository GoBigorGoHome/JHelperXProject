//
// Created by zjsdu on 12/22/2020.
//
#include "print_test.h"
#include "color.h"
#include <iostream>
#include "test.h"
#include "string_utils.h"
#include <cstring>
#include <fstream>

extern std::vector<jhelper::Test> tests;

namespace jhelper {
void print_test(std::ostream &os, int test_id, const std::string &task_output,
                const std::string &diagnostic) {
  std::ifstream debug("solution/debug.txt");
  if (debug.is_open()) {
    // basic_ostream& operator<<( std::basic_streambuf<CharT, Traits>* sb );
    // 文档：https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
    // 注意事项：If no characters were inserted, executes setstate(failbit).
    // e所以在 os << debug.rdbuf(); 之后要调用 os.clear();
    os << debug.rdbuf();
    debug.close();
    os.clear();
  }
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

  os << "Actual output:\n"
     << (line_cnt(task_output) > 2000 ? "TOO LONG, SKIPPED" : task_output)
     << '\n';
  if (!diagnostic.empty()) {
    os << diagnostic << '\n';
  }
  os << BLUE "====================================\n" RESET;
  os.flush();
}

void print_subtest(std::ostream &os, int test_id, int subtest_id, int old_tellg,
                   int new_tellg, const std::vector<std::string> &answer_lines,
                   int n_matched_lines,
                   const std::vector<std::string> &output_lines) {

  os << "Subtest #" << test_id << "." << subtest_id << '\n';
  os << "Input:\n";
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
  if (output_lines.empty()) {
    os << YELLOW "All dumped\n" RESET;
    for (int i = n_matched_lines; i < (int) answer_lines.size(); i++)
      os << answer_lines[i] << '\n';
  } else {
    int end = std::min((int) answer_lines.size(),
                       n_matched_lines + (int) output_lines.size());

    for (int i = n_matched_lines; i < end; i++)
      os << answer_lines[i] << '\n';
  }
  os << '\n';
  os << "Actual output: \n";
  if (output_lines.size() > 2000)
    os << "TOO LONG, SKIPPED.";
  else
    for (auto &line : output_lines)
      os << line << '\n';
  if (output_lines.empty()) {
    os << YELLOW "NULL\n" RESET;
  }
  os << BLUE "====================================\n" RESET;
  os.flush();
}
}// namespace jhelper
