#ifndef LAP_CONFIG_OUTPUT_CONFIG_H
#define LAP_CONFIG_OUTPUT_CONFIG_H

#include "laplace/laplacepre.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  struct OutputConfig {
    int interval = 1;

    bool operator==(OutputConfig other) const;
  };

  std::ostream& operator<<(std::ostream &, const OutputConfig &);
}}

namespace YAML {
  using OutputConfig = laplace::config::OutputConfig;
  template<> struct convert<OutputConfig> {
    static Node encode(const OutputConfig &rhs);
    static bool decode(const Node &node, OutputConfig &rhs);
  };
}

#endif
