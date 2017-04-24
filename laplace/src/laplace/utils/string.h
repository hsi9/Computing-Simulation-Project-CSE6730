#ifndef LAP_UTILS_STRING_H
#define LAP_UTILS_STRING_H

#include <regex>

namespace laplace {
namespace util {
namespace string {
  const std::regex margins_regex("\\n([ ]*)\\|");

  inline std::string strip_margin(const std::string &s) {
    return std::regex_replace(s, margins_regex, "\n");
  }

  inline std::string strip_margin_replace_newlines(const std::string &s) {
    return std::regex_replace(s, margins_regex, " ");
  }

  std::vector<std::string> split(const std::string &s, char delim);

}}}

#endif
