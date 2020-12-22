//
// Created by zjsdu on 12/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TEST_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TEST_H_
namespace jhelper {
struct Test {
  const char *input;
  const char *output;
  bool active;
  bool has_output;
};
enum class TestType { SINGLE, MULTI_NUMBER, MULTI_EOF };
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_TEST_H_
