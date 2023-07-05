//
// Created by zjsdu on 4/27/2020.
//
#include "color.h"
#include "string_utils.h"
#include "test_runner.h"
#include "print_test.h"
#include "checker.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <filesystem>
namespace fs = std::filesystem;

extern std::vector<jhelper::Test> tests;
extern const bool show_all_failed_tests;

#define TIME_CMD                                                               \
  " /usr/bin/time --format=%e --output=solution/time.txt --append "

const char *solution_output_file = "solution/output.txt";
double max_exec_time = 0;

std::string get_file_contents(const char *filename) {
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp) {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return contents;
  }
  throw(errno);
}

bool is_empty(const char *s) {
  while (*s) {
    if (!std::isspace(*s))
      return false;
    s++;
  }
  return true;
}

double get_max_exec_time() {
  std::ifstream in("solution/time.txt");
  double res = 0;
  double t;
  while (in >> t)
    if (t > res)
      res = t;
  in.close();
  std::system(": > solution/time.txt");
  return res;
}

double get_total_exec_time() {
  std::ifstream in("solution/time.txt");
  double sum = 0;
  double t;
  while (in >> t)
    sum += t;
  in.close();
  std::system(": > solution/time.txt");
  return sum;
}

std::string make_command(double time_limit, int stdin_position) {
  return "timeout " + std::to_string(time_limit)
      + "s " TIME_CMD SOLUTION_EXE " " + std::to_string(stdin_position)
      + " < solution/input.txt > solution/output.txt";
}

void write_to_file(const char *s, const char *file) {
  std::ofstream f(file);
  f << s;
}

int get_tellg() {
  std::ifstream f("solution/tellg.txt");
  int x;
  f >> x;
  return x;
}

namespace jhelper {

int run_single_test(int testID, const jhelper::Test &test) {
  const char *command = "timeout 3s" TIME_CMD SOLUTION_EXE
                        " < solution/input.txt > solution/output.txt";
  int status = std::system(command);
  int exit_code = WEXITSTATUS(status);

  std::string solution_output = get_file_contents(solution_output_file);

  if (exit_code == 124) {
    std::cerr << RED "TLE\n" RESET;
    print_test(std::cerr, testID, solution_output);
    return -1;
  }

  if (test.has_output) {
    if (not check(test.output, solution_output)) {
      print_test(std::cerr, testID, solution_output);
      return 1;
    }
    return 0;
  } else {
    print_test(std::cerr, testID, solution_output);
    return -1;
  }
}

/*
 * 一个输入文件里有多个 case 的情形：
 * 把输入文件已经到哪里了这个信息写到一个文件里，每次启动子进程时，从这个位置开始读。
 * 也可以把读操作已经到达的位置传给子进程的main函数。
 */

int run_multi_subtests(int testID, const Test &test, TestType type) {
  int n_subtest = 1000000000;
  int n_read = 0;

  if (type == TestType::MULTI_NUMBER)
    std::sscanf(test.input, "%d%n", &n_subtest, &n_read);

  auto answer_lines = normalize(test.output);

  int n_matched_lines = 0;
  int subtest_id = 1;

  int len = (int) std::strlen(test.input);

  while (n_read < len and subtest_id <= n_subtest) {
    std::string command = make_command(3, n_read);
    int status = std::system(command.c_str());
    int exit_code = WEXITSTATUS(status);
    if (exit_code == 124) {
      std::cerr << RED "TLE on subtest #" << testID << '.' << subtest_id
                << "\n" RESET;
      return -1;
    }

    if (exit_code != 0) {
      std::cerr << RED "RE on subtest #" << testID << '.' << subtest_id
                << ", exit code " << exit_code << "\n" RESET;
      return -1;
    }

    int nn_read = get_tellg();

    auto file_size = fs::file_size(solution_output_file);
    if (file_size > 512000) {
      std::cerr << "Maybe OLE, output " << file_size << " B\n";
      return -1;
    }

    std::string solution_output = get_file_contents(solution_output_file);
    auto outputLines = normalize(solution_output);

    if (not checkLines(outputLines, answer_lines, n_matched_lines)) {
      auto beg = answer_lines.begin() + n_matched_lines;
      auto end = beg
          + std::min<unsigned long>(answer_lines.size() - n_matched_lines,
                                    outputLines.size());
      print_subtest(std::cerr, testID, subtest_id, n_read, nn_read, beg, end,
                    outputLines);
      return 1;
    } else {
      n_matched_lines += (int) outputLines.size();
      ++subtest_id;
      n_read = nn_read;
    }
  }

  max_exec_time = std::max(max_exec_time, get_total_exec_time());

  if (type == TestType::MULTI_NUMBER)
    assert(subtest_id == n_subtest);

  return 0;
}

void run_all_tests(TestType test_type) {
  std::system("> solution/time.txt");// clear time.txt
  int n_active_tests = 0;
  int n_failed_tests = 0;

  for (int i = 0; i < tests.size(); i++)
    if (tests[i].active) {
      ++n_active_tests;
      if (is_empty(tests[i].input)) {
        std::cerr << RED "No input found on test " << i + 1 << ".\n" RESET;
        return;
      }
      // 把 test.input 写到输入文件
      write_to_file(tests[i].input, "solution/input.txt");

      /*
       设想：
      通过管道，把子进程的 stdout 重定向到父进程的 stdin。
             而不是先把子进程的输出写到文件，然后父进程读这个文件。

      忧虑：子进程是不是要等到父进程把它的 stdout 的内容读完才结束？
      */
      int ret = 0;
      if (test_type == TestType::SINGLE)
        ret = run_single_test(i + 1, tests[i]);
      else
        ret = run_multi_subtests(i + 1, tests[i], test_type);

      if (ret == -1)
        return;
      if (ret == 1) {
        n_failed_tests += 1;
        if (!show_all_failed_tests)
          break;
      }
    }

  if (test_type == TestType::SINGLE)
    max_exec_time = get_max_exec_time();

  if (n_active_tests == 0) {
    std::cerr << YELLOW "No active test\n" RESET;
  } else {
    if (n_failed_tests == 0)
      std::cerr << BRIGHT_GREEN "All OK\n" RESET;
    else if (show_all_failed_tests)
      std::cerr << RED "Failed " << n_failed_tests << '/' << n_active_tests
                << " tests\n" RESET;

    std::cerr << BRIGHT_BLACK "Max exec time: " << max_exec_time
              << "s.\n" RESET;
  }
}
}// namespace jhelper
