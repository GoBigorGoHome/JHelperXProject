//
// Created by zjsdu on 12/22/2020.
//
#include "color.h"
#include "checker.h"
#include "string_utils.h"
#include "compare_floats.h"
//#include <sstream>
#include <iostream>

//extern std::ostringstream diagnostic_stream;
extern const bool compare_real_numbers;

namespace jhelper {
// copy from cpeditor
bool checkIgnoreTrailingSpaces(const std::string &output,
                               const std::string &expected) {
  auto outputLines = normalize(output);
  auto answerLines = normalize(expected);

  // if they are considered the same, they must have the same number of lines
  if (outputLines.size() != answerLines.size()) {
    std::cerr << "number of output lines differ:\n";
    std::cerr << "ACTUAL: " << outputLines.size() << "\n";
    std::cerr << "ANSWER: " << answerLines.size() << "\n";
    return false;
  }

  for (int i = 0; i < outputLines.size(); ++i) {
    // if they are considered the same, the current line should be exactly the
    // same after normalization
    if (outputLines[i] != answerLines[i]) {
      std::cerr << "the " << i + 1 << "th line differ.\n";
      return false;
    }
  }

  return true;
}

bool checkLines(const std::vector<std::string> &outputLines,
                const std::vector<std::string> &answerLines, int beg) {
  if (beg + outputLines.size() > answerLines.size()) {
    std::cerr << YELLOW "number of output lines differ:\n" YELLOW;
    std::cerr << "ACTUAL: " << outputLines.size() << "\n";
    std::cerr << "ANSWER: " << answerLines.size() - beg << "\n";
    return false;
  }
  if (compare_real_numbers) {
    for (int i = 0; i < outputLines.size(); ++i) {
      if (not compare_floats(outputLines[i], answerLines[i + beg])) {
        std::cerr << YELLOW "the " << i + 1 << "th line differ.\n" RESET;
        return false;
      }
    }
  } else {
    for (int i = 0; i < outputLines.size(); ++i) {
      if (outputLines[i] != answerLines[i + beg]) {
        std::cerr << YELLOW "the " << i + 1 << "th line differ.\n" RESET;
        return false;
      }
    }
  }
  return true;
}

bool check(std::string expected, std::string actual) {
  if (compare_real_numbers)
    return compare_floats(actual, expected);
  return checkIgnoreTrailingSpaces(actual, expected);
}
}// namespace jhelper