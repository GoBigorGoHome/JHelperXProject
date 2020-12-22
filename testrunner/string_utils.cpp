//
// Created by zjsdu on 12/22/2020.
//
#include "string_utils.h"
#include <iosfwd>
#include <regex>
namespace jhelper {
// copy from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::vector<std::string> normalize(std::string str) {
  // remove trailing spaces
  while (not str.empty() and std::isspace(str.back()))
    str.pop_back();
  // replace \r\n and \r by \n
  auto result =
      std::regex_replace(std::regex_replace(str, std::regex("\r\n"), "\n"),
                         std::regex("\r"), "\n");
  // split into lines
  auto lines = split(result, '\n');
  // remove trailing spaces of each line
  for (auto &line : lines)
    while (not line.empty() and std::isspace(line.back()))
      line.pop_back();
  return lines;
}

int line_cnt(const std::string &s) {
  return (int) std::count_if(s.begin(), s.end(),
                             [](char x) { return x == '\n'; });
}
}// namespace jhelper
