//
// Created by zjsdu on 12/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_PRINT_TEST_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_PRINT_TEST_H_
#include <iosfwd>
#include <string>
#include <vector>
namespace jhelper {
void print_test(std::ostream &os, int test_id, const std::string &task_output,
                const std::string &diagnostic);
void print_subtest(std::ostream &os, int test_id, int subtest_id, int old_tellg,
                   int new_tellg, const std::vector<std::string>& answer_lines,
                   int n_matched_lines,
                   const std::vector<std::string> &output_lines);
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_PRINT_TEST_H_
