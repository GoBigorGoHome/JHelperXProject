//
// Created by zjsdu on 4/27/2020.
//
#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TESTER_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TESTER_H_
#include <string>
#include <vector>
namespace jhelper {

struct Test {
  std::string input;
  std::string output;
  bool active;
  bool has_output;
};
// copy from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string &s, char delimiter);
// copy from cpeditor
bool checkIgnoreTrailingSpaces(const std::string &output,
                               const std::string &expected);
bool check(std::string expected, std::string actual);
void test_runner();
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TESTER_H_
