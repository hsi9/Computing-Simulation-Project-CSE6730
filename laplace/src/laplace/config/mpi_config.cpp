#include "laplace/config/mpi_config.h"

namespace config = laplace::config;
using MPIConfig = laplace::config::MPIConfig;

std::ostream& config::operator<<(std::ostream &os, const MPIConfig &c) {
  YAML::Node node;
  node["MPIConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

bool MPIConfig::operator==(MPIConfig other) const {
  return dimensions[0] == other.dimensions[0] and
         dimensions[1] == other.dimensions[1] and
         dimensions[2] == other.dimensions[2];
}

YAML::Node YAML::convert<MPIConfig>::encode(const MPIConfig &rhs) {
  Node dimensions;
  dimensions.push_back(rhs.dimensions[0]);
  dimensions.push_back(rhs.dimensions[1]);
  dimensions.push_back(rhs.dimensions[2]);

  Node node;
  node["dimensions"] = dimensions;
  return node;
}

bool YAML::convert<MPIConfig>::decode(const Node &node, MPIConfig &rhs) {
  if (not node.IsMap()) {
    return false;
  }

  auto dimensions = node["dimensions"];
  if (!dimensions.IsSequence() or dimensions.size() != laplace::DIMS) {
    return false;
  }

  rhs.dimensions[0] = dimensions[0].as<int>();
  rhs.dimensions[1] = dimensions[1].as<int>();
  rhs.dimensions[2] = dimensions[2].as<int>();
  return true;
}
