#ifndef LAP_CONFIG_SIMULATION_CONFIG_H
#define LAP_CONFIG_SIMULATION_CONFIG_H

#include "laplace/laplacepre.h"
#include "laplace/config/t_coupling_config.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  struct SimulationConfig {
    TCouplingConfig tcoupling;

    bool operator==(SimulationConfig other) const;

    static SimulationConfig load_from_file(const std::string &filepath);
  };

  std::ostream& operator<<(std::ostream &, const SimulationConfig &);
}}

namespace YAML {
  using SimulationConfig = laplace::config::SimulationConfig;
  template<> struct convert<SimulationConfig> {
    static Node encode(const SimulationConfig &rhs);
    static bool decode(const Node &node, SimulationConfig &rhs);
  };
}

#endif
