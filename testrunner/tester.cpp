//
// Created by zjsdu on 4/27/2020.
//
// foreground colors
#define CSI "\033["
#define RESET CSI "0m"
#define BLACK CSI "30m"
#define RED CSI "31m"
#define GREEN CSI "32m"
#define YELLOW CSI "33m"
#define BLUE CSI "34m"
#define MAGENTA CSI "35m"
#define CYAN CSI "36m"
#define WHITE CSI "37m"
#define BRIGHT_BLACK CSI "90m"
#define BRIGHT_RED CSI "91m"
#define BRIGHT_GREEN CSI "92m"
#define BRIGHT_YELLOW CSI "93m"
#define BRIGHT_BLUE CSI "94m"
#define BRIGHT_MAGENTA CSI "95m"
#define BRIGHT_CYAN CSI "96m"
#define BRIGHT_WHITE CSI "97m"
#include "tester.h"
#include <iostream>
#include <sstream>
#include <regex>
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#include <windows.h>
#include <chrono>
#include <future>

void solver_call();
extern std::vector<jhelper::Test> tests;
extern const bool show_all_failed_tests;
std::ostringstream debug_stream;

namespace jhelper {
// copy from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}
// copy from cpeditor
bool checkIgnoreTrailingSpaces(const std::string &output,
                               const std::string &expected) {
  // first, replace \r\n and \r by \n
  auto out =
      std::regex_replace(std::regex_replace(output, std::regex("\r\n"), "\n"),
                         std::regex("\r"), "\n");
  auto ans =
      std::regex_replace(std::regex_replace(expected, std::regex("\r\n"), "\n"),
                         std::regex("\r"), "\n");

  // split output and answer into lines
  auto outputLines = split(out, '\n');
  auto answerLines = split(ans, '\n');

  // if they are considered the same, they must have the same number of lines
  if (outputLines.size() != answerLines.size())
    return false;

  for (int i = 0; i < outputLines.size(); ++i) {
    auto &outputLine = outputLines[i];
    auto &answerLine = answerLines[i];

    // remove trailing spaces at the end of the current line
    while (!outputLine.empty() && isspace(outputLine.back()))
      outputLine.pop_back();
    while (!answerLine.empty() && isspace(answerLine.back()))
      answerLine.pop_back();

    // if they are considered the same, the current line should be exactly the
    // same after removing trailing spaces
    if (outputLine != answerLine)
      return false;
  }

  // all tests are passed, this output is accepted
  return true;
}

bool check(std::string expected, std::string actual) {
  while (!expected.empty() && isspace(*expected.rbegin()))
    expected.pop_back();
  while (!actual.empty() && isspace(*actual.rbegin()))
    actual.pop_back();
  return checkIgnoreTrailingSpaces(actual, expected);
}

bool enable_virtual_terminal() {
  // Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) {
    return false;
  }
  DWORD dwOriginalOutMode = 0;
  if (!GetConsoleMode(hOut, &dwOriginalOutMode)) {
    return false;
  }
  DWORD dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  return SetConsoleMode(hOut, dwOutMode) != 0;
}
int line_cnt(const std::string &s) {
  return std::count_if(s.begin(), s.end(), [](char x) { return x == '\n'; });
}
void print_test(std::ostream &os, int test_id, int exit_code,
                const std::string &task_output = "") {
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

  if (exit_code == 11) {
    os << YELLOW "TIMEOUT\n" RESET;
  } else {
    os << "Actual output: \n"
       << BLACK
       << (line_cnt(task_output) > 2000 ? "TOO LONG, SKIPPED" : task_output)
       << RESET << '\n';
  }
  //当调用std::exit()结束程序时，“all C streams are flushed and closed”
  //我的理解是：std::exit()会调用 std::cout,
  // std::cerr，std::clog的flush()方法，而不管这三个stream
  //指向的buffer究竟是不是对应于stdin/stdout/stderr。
  os.flush();
}

void test_runner() {
  if (!enable_virtual_terminal()) {
    std::cerr << "Enable virtual terminal failed\n";
  }
#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
  debug_stream.precision(10);
  debug_stream << std::fixed << std::boolalpha;
  auto cout_buff = std::cout.rdbuf();
  std::ostream real_cout(cout_buff);
  // to show time in decimal format
  real_cout << std::fixed;
  //////////////////////////////////
  int testID = 0;
  double maxTime = 0.0;
  int n_active_tests = 0;
  int n_failed_tests = 0;
  for (const jhelper::Test &test : tests) {
    if (test.active) {
      ++n_active_tests;
      std::stringstream in(test.input);
      debug_stream.str("");
      std::ostringstream task_out;
      std::cin.rdbuf(in.rdbuf());
      std::cout.rdbuf(task_out.rdbuf());

      std::packaged_task<double()> task([]() {
        auto start = std::chrono::steady_clock::now();
        solver_call();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        return elapsed_seconds.count();
      });
      auto res = task.get_future();
      std::thread task_tread(std::move(task));
      task_tread.detach();
      auto status = res.wait_for(std::chrono::seconds(3));
      if (status == std::future_status::timeout) {
        print_test(real_cout, testID, 11);
        std::exit(11);
      }
      maxTime = std::max(res.get(), maxTime);
      auto task_output = task_out.str();
      if (test.has_output && not jhelper::check(test.output, task_output)
          or not test.has_output) {
        print_test(real_cout, testID, 0, task_output);
        if (not show_all_failed_tests)
          std::exit(0);
        ++n_failed_tests;
      }
    }
    ++testID;
  }

  if (n_active_tests == 0) {
    real_cout << YELLOW << "No active test\n" << RESET;
  } else if (n_failed_tests == 0) {
    real_cout << BRIGHT_GREEN "All OK\n" RESET;
    real_cout << BRIGHT_BLACK "Maximal time: " << maxTime << "s.\n" RESET;
  } else {
    real_cout << RED "Failed " << n_failed_tests << '/' << n_active_tests
              << " test(s)\n";
  }
  real_cout.flush();
  std::exit(0);
}

void display_color() {
  std::cout << RED "RED\n";
  std::cout << BRIGHT_RED "BRIGHT RED\n";
  std::cout << GREEN "GREEN\n";
  std::cout << BRIGHT_GREEN "BRIGHT GREEN\n";
  std::cout << YELLOW "YELLOW\n";
  std::cout << BRIGHT_YELLOW "BRIGHT_YELLOW\n";
  std::cout << BLUE << "BLUE\n";
  std::cout << BRIGHT_BLUE << "BRIGHT BLUE\n";
  std::cout << MAGENTA << "MAGENTA\n";
  std::cout << BRIGHT_MAGENTA << "BRIGHT MAGENTA\n";
  std::cout << CYAN << "CYAN\n";
  std::cout << BRIGHT_CYAN << "BRIGHT_CYAN\n";
  std::cout << BLACK << "BLACK\n";
  std::cout << BRIGHT_BLACK << "BRIGHT BLACK\n";
  std::cout << WHITE << "WHITE\n";
  std::cout << BRIGHT_WHITE << "BRIGHT WHITE\n";
}
}// namespace jhelper
