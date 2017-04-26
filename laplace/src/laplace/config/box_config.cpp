#include "laplace/config/box_config.h"

using namespace laplace;
using BoxConfig = laplace::config::BoxConfig;

constexpr auto _L = "L";
constexpr auto _CELL_DIMS = "cell-dims";

std::ostream& config::operator<<(std::ostream &os, const BoxConfig &c) {
  YAML::Node node;
  node["BoxConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

bool BoxConfig::operator==(BoxConfig other) const {
  return L[0] == other.L[0] and
         L[1] == other.L[1] and
         L[2] == other.L[2] and
         celldims[0] == other.celldims[0] and
         celldims[1] == other.celldims[1] and
         celldims[2] == other.celldims[2];
}

YAML::Node YAML::convert<BoxConfig>::encode(const BoxConfig &c) {
  Node l;
  l.push_back(c.L[0]);
  l.push_back(c.L[1]);
  l.push_back(c.L[2]);

  Node cd;
  cd.push_back(c.celldims[0]);
  cd.push_back(c.celldims[1]);
  cd.push_back(c.celldims[2]);

  Node node;
  node[_L] = l;
  node[_CELL_DIMS] = cd;
  return node;
}

bool YAML::convert<BoxConfig>::decode(const Node &node, BoxConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto l = node[_L];
  if (!l.IsSequence() or l.size() != laplace::DIMS) {
    return false;
  }
  c.L[0] = l[0].as<real>();
  c.L[1] = l[1].as<real>();
  c.L[2] = l[2].as<real>();

  auto cd = node[_CELL_DIMS];
  if (!cd.IsSequence() or cd.size() != laplace::DIMS) {
    return false;
  }
  c.celldims[0] = cd[0].as<int>();
  c.celldims[1] = cd[1].as<int>();
  c.celldims[2] = cd[2].as<int>();

  return true;
}
