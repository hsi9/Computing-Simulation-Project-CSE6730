#ifndef LAP_CONFIG_BOX_CONFIG_H
#define LAP_CONFIG_BOX_CONFIG_H

#include "laplace/laplacepre.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  struct BoxConfig {
    real L[DIMS] = { 0, 0, 0 };
    int celldims[DIMS] = { 0, 0, 0 };

    bool operator==(BoxConfig other) const;
  };

  std::ostream& operator<<(std::ostream &, const BoxConfig &);
}}

namespace YAML {
  using BoxConfig = laplace::config::BoxConfig;
  template<> struct convert<BoxConfig> {
    static Node encode(const BoxConfig &rhs);
    static bool decode(const Node &node, BoxConfig &rhs);
  };
}

#endif
