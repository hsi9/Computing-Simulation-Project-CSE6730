#ifndef LAP_CONFIG_T_COUPLING_CONFIG_H
#define LAP_CONFIG_T_COUPLING_CONFIG_H

#include "laplace/laplacepre.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  enum class ThermostatType {
    NONE,
    SCALING,
    HOOVER_EVANS,
    DUMMY
  };

  inline const char* enumToString(ThermostatType t) {
    switch (t) {
      case ThermostatType::SCALING: return "SCALING";
      case ThermostatType::HOOVER_EVANS: return "HOOVER_EVANS";
      case ThermostatType::DUMMY: return "DUMMY";
      default: return "NONE";
    }
  }

  struct TCouplingConfig {
    ThermostatType thermostat = ThermostatType::NONE;
    int coupling_freq = -1;
    int nh_chain_length = 10;

    bool operator==(TCouplingConfig other) const;
  };

  std::ostream& operator<<(std::ostream &, const TCouplingConfig &);
}}

namespace YAML {
  using TCouplingConfig = laplace::config::TCouplingConfig;
  template<> struct convert<TCouplingConfig> {
    static Node encode(const TCouplingConfig &rhs);
    static bool decode(const Node &node, TCouplingConfig &rhs);
  };
}

#endif
