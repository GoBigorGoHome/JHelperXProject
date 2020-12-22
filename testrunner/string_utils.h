//
// Created by zjsdu on 12/22/2020.
//

#ifndef JHELPER_EXAMPLE_PROJECT_TESTRUNNER_STRING_UTILS_H_
#define JHELPER_EXAMPLE_PROJECT_TESTRUNNER_STRING_UTILS_H_
#include <vector>
#include <string>

namespace jhelper {
std::vector<std::string> split(const std::string &s, char delimiter);
std::vector<std::string> normalize(std::string str);
int line_cnt(const std::string &s);
}// namespace jhelper
#endif// JHELPER_EXAMPLE_PROJECT_TESTRUNNER_STRING_UTILS_H_
