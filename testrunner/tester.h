//
// Created by zjsdu on 4/27/2020.
//
#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TESTER_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TESTER_H_
namespace jhelper {
struct Test {
  const char* input;
  const char* output;
  bool active;
  bool has_output;
};
void test_runner();
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TESTER_H_
