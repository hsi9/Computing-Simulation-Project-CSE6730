#include "laplace/config/t_coupling_config.h"

namespace config = laplace::config;
using TCouplingConfig = laplace::config::TCouplingConfig;

std::ostream& config::operator<<(std::ostream &os, const TCouplingConfig &c) {
  return os << "TCouplingConfig {"
            << "\n  thermostat: " << enumToString(c.thermostat)
            << "\n  coupling_freq: " << c.coupling_freq
            << "\n  nh_chain_length: " << c.nh_chain_length
            << "\n}";
}

bool TCouplingConfig::operator==(TCouplingConfig other) const {
  return thermostat == other.thermostat and
         coupling_freq == other.coupling_freq and
         nh_chain_length == other.nh_chain_length;
}

YAML::Node YAML::convert<TCouplingConfig>::encode(const TCouplingConfig &c) {
  Node node;
  node["thermostat"] = enumToString(c.thermostat);
  node["coupling_freq"] = c.coupling_freq;
  node["nh_chain_length"] = c.nh_chain_length;
  return node;
}

bool YAML::convert<TCouplingConfig>::decode(const Node &node, TCouplingConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto thermostat = node["thermostat"];
  if (thermostat.IsScalar()) {
    auto s = thermostat.as<std::string>();
    if (s == "SCALING") c.thermostat = config::ThermostatType::SCALING;
    else if (s == "HOOVER_EVANS") c.thermostat = config::ThermostatType::HOOVER_EVANS;
    else if (s == "DUMMY") c.thermostat = config::ThermostatType::DUMMY;
  } else {
    return false;
  }

  auto coupling_freq = node["coupling_freq"];
  if (coupling_freq.IsScalar()) {
    c.coupling_freq = coupling_freq.as<int>();
  } else {
    return false;
  }

  auto nh_chain_length = node["nh_chain_length"];
  if (nh_chain_length.IsScalar()) {
    c.nh_chain_length = nh_chain_length.as<int>();
  } else {
    return false;
  }

  return true;
}
