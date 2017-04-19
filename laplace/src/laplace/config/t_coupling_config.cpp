#include "laplace/config/t_coupling_config.h"

namespace config = laplace::config;
using TCouplingConfig = laplace::config::TCouplingConfig;

constexpr auto _THERMOSTAT = "thermostat";
constexpr auto _COUPLING_FREQ = "coupling-freq";
constexpr auto _NH_CHAIN_LENGTH = "nh-chain-length";

std::ostream& config::operator<<(std::ostream &os, const TCouplingConfig &c) {
  YAML::Node node;
  node["TCouplingConfig"] = c;
  YAML::Emitter out;
  out << node;
  return os << out.c_str();
}

bool TCouplingConfig::operator==(TCouplingConfig other) const {
  return thermostat == other.thermostat and
         coupling_freq == other.coupling_freq and
         nh_chain_length == other.nh_chain_length;
}

YAML::Node YAML::convert<TCouplingConfig>::encode(const TCouplingConfig &c) {
  Node node;
  node[_THERMOSTAT] = enumToString(c.thermostat);
  node[_COUPLING_FREQ] = c.coupling_freq;
  node[_NH_CHAIN_LENGTH] = c.nh_chain_length;
  return node;
}

bool YAML::convert<TCouplingConfig>::decode(const Node &node, TCouplingConfig &c) {
  if (not node.IsMap()) {
    return false;
  }

  auto thermostat = node[_THERMOSTAT];
  if (thermostat.IsScalar()) {
    auto s = thermostat.as<std::string>();
    if (s == "SCALING") c.thermostat = config::ThermostatType::SCALING;
    else if (s == "HOOVER_EVANS") c.thermostat = config::ThermostatType::HOOVER_EVANS;
    else if (s == "DUMMY") c.thermostat = config::ThermostatType::DUMMY;
  } else {
    return false;
  }

  auto coupling_freq = node[_COUPLING_FREQ];
  if (coupling_freq.IsScalar()) {
    c.coupling_freq = coupling_freq.as<int>();
  } else {
    return false;
  }

  auto nh_chain_length = node[_NH_CHAIN_LENGTH];
  if (nh_chain_length.IsScalar()) {
    c.nh_chain_length = nh_chain_length.as<int>();
  } else {
    return false;
  }

  return true;
}
