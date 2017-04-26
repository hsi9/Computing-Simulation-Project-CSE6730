#include "laplace/config/ns_config.h"

using namespace laplace;
using NeighborSearchConfig = laplace::config::NeighborSearchConfig;

constexpr auto _RCUT = "rcut";
constexpr auto _RSHELL = "rshell";
constexpr auto _INTERVAL = "interval";

std::ostream& config::operator<<(std::ostream &os, const NeighborSearchConfig &c) {
  YAML::Node node;
  node["NeighborSearchConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

bool NeighborSearchConfig::operator==(NeighborSearchConfig other) const {
  return rcut == other.rcut and
         rshell == other.rshell and
         interval == other.interval;
}

YAML::Node YAML::convert<NeighborSearchConfig>::encode(const NeighborSearchConfig &c) {
  Node node;
  node[_RCUT] = c.rcut;
  node[_RSHELL] = c.rshell;
  node[_INTERVAL] = c.interval;
  return node;
}

bool YAML::convert<NeighborSearchConfig>::decode(const Node &node, NeighborSearchConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto rcut = node[_RCUT];
  if (rcut.IsScalar()) {
    c.rcut = rcut.as<real>();
  } else {
    return false;
  }

  auto rshell = node[_RSHELL];
  if (rshell.IsScalar()) {
    c.rshell = rshell.as<real>();
  } else {
    return false;
  }

  auto interval = node[_INTERVAL];
  if (interval.IsScalar()) {
    c.interval = interval.as<int>();
  } else {
    return false;
  }

  return true;
}
