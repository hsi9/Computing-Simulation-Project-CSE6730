#ifndef LAP_CONFIG_NS_CONFIG_H
#define LAP_CONFIG_NS_CONFIG_H

#include "laplace/laplacepre.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  struct NeighborSearchConfig {
    real rcut = 1.0;
    real rshell = 5.0;
    int interval = 1;

    bool operator==(NeighborSearchConfig other) const;
  };

  std::ostream& operator<<(std::ostream &, const NeighborSearchConfig &);
}}

namespace YAML {
  using NeighborSearchConfig = laplace::config::NeighborSearchConfig;
  template<> struct convert<NeighborSearchConfig> {
    static Node encode(const NeighborSearchConfig &rhs);
    static bool decode(const Node &node, NeighborSearchConfig &rhs);
  };
}

#endif
