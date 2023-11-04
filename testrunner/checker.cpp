//
// Created by zjsdu on 12/22/2020.
//
#include "color.h"
#include "checker.h"
#include "string_utils.h"
#include "compare_floats.h"
#include <sstream>
#include <iostream>

// extern std::ostringstream diagnostic_stream;
extern const bool compare_real_numbers;

namespace jhelper {
// copy from cpeditor
// 返回比较的结果。若返回值是空串，表示没有异常。
std::string checkIgnoreTrailingSpaces(const std::string &output,
                                      const std::string &expected) {
  auto outputLines = normalize(output);
  auto answerLines = normalize(expected);

  // if they are considered the same, they must have the same number of lines
  std::ostringstream res;
  if (outputLines.size() != answerLines.size()) {
    res << "number of output lines differ:\n";
    res << "ACTUAL: " << outputLines.size() << "\n";
    res << "ANSWER: " << answerLines.size() << "\n";
    return res.str();
  }

  for (int i = 0; i < outputLines.size(); ++i) {
    // if they are considered the same, the current line should be exactly the
    // same after normalization
    if (outputLines[i] != answerLines[i]) {
      res << "the " << i + 1 << "th line differ.\n";
      return res.str();
    }
  }

  return "";
}

bool check_subtest(const std::vector<std::string> &outputLines,
                   const std::vector<std::string> &answerLines,
                   int n_matched_lines) {
  if (outputLines.empty()) {
    std::cerr << YELLOW "No output found, is your solution complete?\n" RESET;
    return false;
  }

  if (n_matched_lines + outputLines.size() > answerLines.size()) {
    std::cerr << YELLOW "number of output lines differ:\n" RESET;
    std::cerr << "ACTUAL: " << outputLines.size() << "\n";
    std::cerr << "ANSWER: " << answerLines.size() - n_matched_lines << "\n";
    return false;
  }
  if (compare_real_numbers) {
    for (int i = 0; i < outputLines.size(); ++i) {
      if (not compare_floats(outputLines[i],
                             answerLines[i + n_matched_lines])) {
        std::cerr << YELLOW "the " << i + 1 << "th line differ.\n" RESET;
        return false;
      }
    }
  } else {
    for (int i = 0; i < outputLines.size(); ++i) {
      if (outputLines[i] != answerLines[i + n_matched_lines]) {
        std::cerr << YELLOW "the " << i + 1 << "th line differ.\n" RESET;
        return false;
      }
    }
  }
  return true;
}

std::string check(std::string expected, std::string actual) {
  if (compare_real_numbers)
    return compare_floats(actual, expected) ? ""
                                            : "Floating-point check failed.";
  return checkIgnoreTrailingSpaces(actual, expected);
}
}// namespace jhelper