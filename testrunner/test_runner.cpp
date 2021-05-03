//
// Created by zjsdu on 4/27/2020.
//
#include "color.h"
#include "string_utils.h"
#include "test_runner.h"
#include "print_test.h"
#include "checker.h"
#include <iostream>
#include <sstream>
#include <regex>
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <windows.h>
#include <chrono>
#include <future>
#include <cassert>
#ifdef _MSC_VER
int MyReportHook(int reportType, char *message, int *returnValue) {
  if (reportType == _CRT_ASSERT) {
    std::cerr << message << '\n';
    std::_Exit(EXIT_FAILURE);
  }
  return FALSE;
}
#else
#include <csignal>
void signal_handler(int signal) {
  std::_Exit(EXIT_FAILURE);
}
#endif

void solver_call();
void solve();
extern std::vector<jhelper::Test> tests;
extern const bool show_all_failed_tests;
std::ostringstream debug_stream;
std::ostringstream diagnostic_stream;

namespace jhelper {
bool enable_virtual_terminal() {
  // Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) {
    std::cerr << "invalid handle value\n";
    return false;
  }
  DWORD dwOriginalOutMode = 0;
  if (!GetConsoleMode(hOut, &dwOriginalOutMode)) {
    std::cerr << "failed GetConsoleMode\n";
    return false;
  }
  if ((dwOriginalOutMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == 0) {
    DWORD dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (SetConsoleMode(hOut, dwOutMode) == 0) {
      std::cerr << "failed SetConsoleMode\n";
      return false;
    }
    return true;
  }
  return true;
}

void test_runner(TestType testType) {
  if (!IsDebuggerPresent()) {
    if (!enable_virtual_terminal()) {
      std::cerr << "Enable virtual terminal failed\n";
    }
  }
#ifdef _MSC_VER
  _CrtSetReportHook(MyReportHook);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#else
  auto previous_handler = std::signal(SIGABRT, signal_handler);
  if (previous_handler == SIG_ERR) {
    std::cerr << "Setup failed\n";
    std::_Exit(EXIT_FAILURE);
  }
#endif
  //  set format for output streams
  debug_stream.precision(10);
  debug_stream << std::fixed << std::boolalpha;
  std::cerr.precision(10);
  std::cerr << std::fixed << std::boolalpha;
  auto cout_buff = std::cout.rdbuf();
  auto cin_buff = std::cin.rdbuf();
  std::ostream real_cout(cout_buff);
  // to show time in decimal format
  real_cout << std::fixed;
  auto reset_streams = [&cout_buff, &cin_buff]() {
    std::cout.rdbuf(cout_buff);
    std::cin.rdbuf(cin_buff);
  };
  //////////////////////////////////
  constexpr std::chrono::seconds time_limit(3);
  int testID = 0;
  double maxTime = 0.0;
  int n_active_tests = 0;
  int n_failed_tests = 0;
  for (const jhelper::Test &test : tests) {
    if (test.active) {
      ++n_active_tests;
      std::stringstream in(test.input);
      while (in.good() and isspace(in.peek())) {
        in.get();
      }
      if (not in.good()) {
        real_cout << RED "No input found on test " << testID << ".\n" RESET;
        real_cout.flush();
        reset_streams();
        return;
      }
      debug_stream.str("");
      diagnostic_stream.str("");
      std::ostringstream task_out;
      std::cin.rdbuf(in.rdbuf());
      std::cout.rdbuf(task_out.rdbuf());
      if (testType == TestType::SINGLE or show_all_failed_tests
          or not test.has_output) {
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
        auto status = res.wait_for(time_limit);
        if (status == std::future_status::timeout) {
          print_test(real_cout, testID, "N/A");
          real_cout << RED "TLE on test " << testID << "\n" RESET;
          real_cout.flush();
          reset_streams();
          return;
        }
        maxTime = std::max(res.get(), maxTime);
        auto task_output = task_out.str();
        if (test.has_output) {
          if (not jhelper::check(test.output, task_output)) {
            print_test(real_cout, testID, task_output);
            if (not show_all_failed_tests) {
              real_cout << RED "WA on test " << testID << "\n" RESET;
              real_cout << BRIGHT_BLACK "Maximal time: " << maxTime
                        << "s.\n" RESET;
              real_cout.flush();
              reset_streams();
              return;
            }
            ++n_failed_tests;
          }
        } else {
          print_test(real_cout, testID, task_output);
          real_cout << YELLOW "End of test " << testID << "\n" RESET;
        }
      } else {
        int n_subtest = INT_MAX;
        if (testType == TestType::MULTI_NUMBER)
          std::cin >> n_subtest;
        while (std::cin.good() and std::isspace(std::cin.peek()))
          std::cin.get();
        auto input_pos = std::cin.tellg();
        auto answer_lines = normalize(test.output);
        int n_matched_lines = 0;
        double total_time = 0;
        int subtest_id = 0;
        while (std::cin.good() and subtest_id < n_subtest) {
          std::packaged_task<double()> task([]() {
            auto start = std::chrono::steady_clock::now();
            solve();
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            return elapsed_seconds.count();
          });
          auto res = task.get_future();
          std::thread task_tread(std::move(task));
          task_tread.detach();
          auto status = res.wait_for(time_limit);
          if (status == std::future_status::timeout) {
            print_subtest(real_cout, testID, subtest_id, input_pos,
                          answer_lines.cbegin() + n_matched_lines,
                          answer_lines.end(), "N/A");
            real_cout << RED "TLE on subtest " << testID << '.' << subtest_id
                      << "\n" RESET;
            real_cout.flush();
            reset_streams();
            return;
          }
          std::string task_output = task_out.str();
          auto outputLines = normalize(task_output);
          if (outputLines.empty()) {
            print_subtest(real_cout, testID, subtest_id, input_pos,
                          answer_lines.cbegin() + n_matched_lines,
                          answer_lines.cend(), task_output);
            real_cout << RED "No output on subtest " << testID << '.'
                      << subtest_id << "\n" RESET;
            real_cout.flush();
            reset_streams();
            return;
          }
          if (not checkLines(outputLines, answer_lines, n_matched_lines)) {
            if (n_matched_lines + outputLines.size() > answer_lines.size())
              print_subtest(real_cout, testID, subtest_id, input_pos,
                            answer_lines.cbegin() + n_matched_lines,
                            answer_lines.cend(), task_output);
            else
              print_subtest(real_cout, testID, subtest_id, input_pos,
                            answer_lines.cbegin() + n_matched_lines,
                            answer_lines.cbegin() + n_matched_lines
                                + outputLines.size(),
                            task_output);
            real_cout << RED "WA on subtest " << testID << '.' << subtest_id
                      << "\n" RESET;
            real_cout.flush();
            reset_streams();
            return;
          } else {
            n_matched_lines += (int) outputLines.size();
            total_time += res.get();
            ++subtest_id;
            debug_stream.str("");
            diagnostic_stream.str("");
            task_out.str("");
            while (std::cin.good() and std::isspace(std::cin.peek()))
              std::cin.get();
            input_pos = std::cin.tellg();
          }
        }
        if (testType == TestType::MULTI_NUMBER)
          assert(subtest_id == n_subtest);
        maxTime = std::max(maxTime, total_time);
      }
    }
    ++testID;
  }

  if (n_active_tests == 0) {
    real_cout << YELLOW "No active test\n" RESET;
  } else {
    if (n_failed_tests == 0) {
      real_cout << BRIGHT_GREEN "All OK\n" RESET;
    } else {
      real_cout << RED "Failed " << n_failed_tests << '/' << n_active_tests
                << " test(s)\n" RESET;
    }
    real_cout << BRIGHT_BLACK "Maximal time: " << maxTime << "s.\n" RESET;
  }
  real_cout.flush();
  reset_streams();
}
}// namespace jhelper
