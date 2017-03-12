#ifndef LAP_CONFIG_MPI_CONFIG_H
#define LAP_CONFIG_MPI_CONFIG_H

#include "laplace/laplacepre.h"
#include <yaml-cpp/yaml.h>
#include <ostream>

namespace laplace {
namespace config {
  struct MPIConfig {
    int dimensions[DIMS] = { 0, 0, 0 };

    bool operator==(MPIConfig other) const;
  };

  std::ostream& operator<<(std::ostream &, const MPIConfig &);
}}

namespace YAML {
  using MPIConfig = laplace::config::MPIConfig;
  template<> struct convert<MPIConfig> {
    static Node encode(const MPIConfig &rhs);
    static bool decode(const Node &node, MPIConfig &rhs);
  };
}

#endif
