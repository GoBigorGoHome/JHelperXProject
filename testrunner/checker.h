//
// Created by zjsdu on 12/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_CHECKER_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_CHECKER_H_
#include <string>
#include <vector>
namespace jhelper {
bool check(std::string expected, std::string actual);
bool checkLines(const std::vector<std::string> &outputLines,
                const std::vector<std::string> &answerLines, int beg);
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_CHECKER_H_
