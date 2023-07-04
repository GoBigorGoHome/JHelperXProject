//
// Created by zjsdu on 12/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_PRINT_TEST_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_PRINT_TEST_H_
#include <iosfwd>
#include <string>
#include <vector>
namespace jhelper {
void print_test(std::ostream &os, int test_id, const std::string &task_output);
void print_subtest(std::ostream &os, int test_id, int subtest_id, int old_tellg,
                   int new_tellg, std::vector<std::string>::const_iterator b,
                   std::vector<std::string>::const_iterator e,
                   const std::vector<std::string> &output_lines);
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_PRINT_TEST_H_
