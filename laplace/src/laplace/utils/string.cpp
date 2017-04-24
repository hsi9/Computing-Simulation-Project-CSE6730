#include "laplace/utils/string.h"
#include <sstream>

namespace stringutil = laplace::util::string;
using namespace std;

vector<std::string> stringutil::split(const std::string &s, char delim) {
  stringstream ss(s);
  std::string item;
  vector<std::string> tokens;
  while (getline(ss, item, delim)) {
    tokens.emplace_back(item);
  }
  return tokens;
}
