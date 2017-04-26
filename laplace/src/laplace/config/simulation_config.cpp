#include "laplace/config/simulation_config.h"

namespace config = laplace::config;
using SimulationConfig = laplace::config::SimulationConfig;

constexpr auto _TCOUPLING = "tcoupling";
constexpr auto _RUN = "run";
constexpr auto _NEIGHBOR_SEARCH = "neighbor-search";
constexpr auto _BOX = "box";
constexpr auto _OUTPUT = "output";

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
  YAML::convert<SimulationConfig>::decode(file, config);
  return config;
}

bool SimulationConfig::operator==(SimulationConfig other) const {
  return tcoupling == other.tcoupling and
         run == other.run and
         neighbor_search == other.neighbor_search and
         box == other.box and
         output == other.output;
}

YAML::Node YAML::convert<SimulationConfig>::encode(const SimulationConfig &c) {
  Node node;
  node[_TCOUPLING] = c.tcoupling;
  node[_RUN] = c.run;
  node[_NEIGHBOR_SEARCH] = c.neighbor_search;
  node[_BOX] = c.box;
  node[_OUTPUT] = c.output;
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

  auto run = node[_RUN];
  if (run.IsMap()) {
    c.run = run.as<RunConfig>();
  } else {
    return false;
  }

  auto ns = node[_NEIGHBOR_SEARCH];
  if (ns.IsMap()) {
    c.neighbor_search = ns.as<NeighborSearchConfig>();
  } else {
    return false;
  }

  auto box = node[_BOX];
  if (box.IsMap()) {
    c.box = box.as<BoxConfig>();
  } else {
    return false;
  }

  auto output = node[_OUTPUT];
  if (output.IsMap()) {
    c.output = output.as<OutputConfig>();
  } else {
    return false;
  }

  return true;
}
