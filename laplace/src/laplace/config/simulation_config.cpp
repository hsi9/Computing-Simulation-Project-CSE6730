#include "laplace/config/simulation_config.h"

namespace config = laplace::config;
using SimulationConfig = laplace::config::SimulationConfig;

constexpr auto _TCOUPLING = "tcoupling";

std::ostream& config::operator<<(std::ostream &os, const SimulationConfig &c) {
  YAML::Node node;
  node["SimulationConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

SimulationConfig SimulationConfig::load_from_file(const std::string &filepath) {
  SimulationConfig config;
  YAML::Node file = YAML::LoadFile(filepath);
  config.tcoupling = file[_TCOUPLING].as<config::TCouplingConfig>();
  return config;
}

bool SimulationConfig::operator==(SimulationConfig other) const {
  return tcoupling == other.tcoupling;
}

YAML::Node YAML::convert<SimulationConfig>::encode(const SimulationConfig &c) {
  Node node;
  node[_TCOUPLING] = c.tcoupling;
  return node;
}

bool YAML::convert<SimulationConfig>::decode(const Node &node, SimulationConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto tcoupling = node[_TCOUPLING];
  if (tcoupling.IsMap()) {
    c.tcoupling = tcoupling.as<TCouplingConfig>();
  } else {
    return false;
  }

  return true;
}
